#include "mdns_task.h"

// Global flag to indicate if the target device is reachable
bool isTargetDeviceAvailable = false;
static unsigned long lastResolutionAttempt = 0;
static const unsigned long RESOLUTION_INTERVAL = 10000;  // Try to resolve every 10 seconds




void createMdnsTask() {
  


  if(xSemaphoreTake(xEthernetMutex, portMAX_DELAY) != pdTRUE) {
    Serial1.println("[MDNS Task] Failed to take Ethernet mutex during initialization!");
    vTaskDelete(NULL);
    return;
  }

  
  // Init
  if (!mdns.begin(Ethernet.localIP(), MDNS_NAME)) {
    Serial1.println("- [X] Failed to initialize MDNS!");
    xSemaphoreGive(xEthernetMutex);
    vTaskDelete(NULL);
    return;
  }

  char SERVICE_NAME[30]; // Used for pretty mDNS service name like "ProjectDMX xxxx" (._http part is not shown on mDNS clients but required)
  snprintf(SERVICE_NAME, sizeof(SERVICE_NAME), "ProjectDMX %02x%02x._http", MAC_ADDRESS[4], MAC_ADDRESS[5]);
  if (!mdns.addServiceRecord(SERVICE_NAME, 80, MDNSServiceTCP)) {
    Serial1.println("- [X] Failed to initialize MDNS: mDNS HTTP service registration failed!");
    xSemaphoreGive(xEthernetMutex);
    vTaskDelete(NULL);
    return;
  }
  Serial1.println("[MDNS Task] MDNS server started");
  

  xSemaphoreGive(xEthernetMutex);
  

  xTaskCreate(
    mdnsTask,        // Task function
    "Mdns",         // Task name
    MDNS_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    MDNS_TASK_PRIORITY,      // Priority
    NULL
  );
  
}


void mdnsTask(void *pvParameters) {
  while (1) {

    // Skip if mutex not available
    if (xSemaphoreTake(xEthernetMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }

    
    mdns.run();
    
    // Periodically try to reach the target device to check if it's available
    unsigned long currentTime = millis();
    if ((currentTime - lastResolutionAttempt) > RESOLUTION_INTERVAL) {
      lastResolutionAttempt = currentTime;
      // Try to connect as a way to verify the name resolves and device is reachable
      EthernetClient testClient;
      testClient.setTimeout(200);
      
      Serial1.println("[MDNS Task] Attempting to resolve pdmx-controller.local...");
      
      if (testClient.connect("pdmx-controller.local", 80)) {
        // Try to send a small amount of data to verify the connection is real
        testClient.println("GET / HTTP/1.1");
        
        // Wait a moment for the remote to respond
        vTaskDelay(pdMS_TO_TICKS(100));
        
        // Check if connection is still active
        if (testClient.connected()) {
          isTargetDeviceAvailable = true;
          
          // Get and print the resolved IP
          uint8_t remoteIP[4];
          testClient.remoteIP(remoteIP);
          
          // Validate the IP isn't a broadcast or null address
          if ((remoteIP[0] != 255 || remoteIP[1] != 255 || remoteIP[2] != 255 || remoteIP[3] != 255) &&
              (remoteIP[0] != 0 || remoteIP[1] != 0 || remoteIP[2] != 0 || remoteIP[3] != 0)) {
            Serial1.print("[MDNS Task] pdmx-controller.local resolved to ");
            Serial1.print(remoteIP[0]);
            Serial1.print(".");
            Serial1.print(remoteIP[1]);
            Serial1.print(".");
            Serial1.print(remoteIP[2]);
            Serial1.print(".");
            Serial1.println(remoteIP[3]);
          } else {
            Serial1.println("[MDNS Task] Connection made but received invalid IP address");
            isTargetDeviceAvailable = false;
          }
        } else {
          isTargetDeviceAvailable = false;
          Serial1.println("[MDNS Task] Connection dropped - device not responding");
        }
        testClient.stop();
      } else {
        isTargetDeviceAvailable = false;
        Serial1.println("[MDNS Task] Failed to connect - pdmx-controller.local does not exist or is unreachable");
      }
    }



    // Give mutex back
    xSemaphoreGive(xEthernetMutex);

    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(1000));
    
  }
}