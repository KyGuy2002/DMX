#include "mdns_task.h"




void createMdnsTask() {
  


  if(xSemaphoreTake(xEthernetMutex, portMAX_DELAY) != pdTRUE) {
    Serial1.println("[MDNS Task] Failed to take Ethernet mutex during initialization!");
    vTaskDelete(NULL);
    return;
  }

  
  // Init
  if (!mdns.begin(Ethernet.localIP(), MDNS_NAME)) {
    Serial1.println("- [X] Failed to initialize MDNS!");
    vTaskDelete(NULL);
    return;
  }

  char SERVICE_NAME[30]; // Used for pretty mDNS service name like "ProjectDMX xxxx" (._http part is not shown on mDNS clients but required)
  snprintf(SERVICE_NAME, sizeof(SERVICE_NAME), "ProjectDMX %02x%02x._http", MAC_ADDRESS[4], MAC_ADDRESS[5]);
  if (!mdns.addServiceRecord(SERVICE_NAME, 80, MDNSServiceTCP)) {
    Serial1.println("- [X] Failed to initialize MDNS: mDNS HTTP service registration failed!");
    vTaskDelete(NULL);
    return;
  }
  Serial1.println("[MDNS Task] MDNS server started");
  

  xSemaphoreGive(xEthernetMutex);
  

  xTaskCreate(
    mdnsTask,        // Task function
    "Mdns",         // Task name
    WEB_TASK_STACK_SIZE,    // Stack size (bytes) - larger for web processing
    NULL,                     // Parameters
    WEB_TASK_PRIORITY,      // Priority (higher for web)
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

    
    Serial1.println("[MDNS Task] Running MDNS task...");
    mdns.run();



    // Give mutex back
    xSemaphoreGive(xEthernetMutex);

    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(1000));
    
  }
}