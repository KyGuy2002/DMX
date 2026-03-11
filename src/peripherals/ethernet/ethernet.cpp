#include "ethernet.h"


EthernetUDP udp;
char MDNS_NAME[10];


void createEthernetInitTask() {
  xTaskCreate(
    ethernetInitTask,             // Task function
    "Ethernet Init",              // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void ethernetInitTask(void *pvParameters) {


  // Wait for SD init
  if (!initSyncWaitOk(INIT_AUDIO_DONE, INIT_AUDIO_OK)) {
    Serial1.println("- [X] Failed to initialize Ethernet: Audio init timeout");
    initSyncMarkDone(INIT_ETHERNET_DONE, false, INIT_ETHERNET_OK);
    vTaskDelete(NULL);
    return;
  }


  // Generate MDNS name (used for url/hostname [pdmx-xxxx.local])
  snprintf(MDNS_NAME, sizeof(MDNS_NAME), "pdmx-%02x%02x", MAC_ADDRESS[4], MAC_ADDRESS[5]);
  
  Ethernet.setCsPin(ETH_CS_PIN);
  Ethernet.setHostname(MDNS_NAME);
  
  if (Ethernet.begin(MAC_ADDRESS) == 0) {
    Serial1.println("- [X] Failed to initialize Ethernet: DHCP failed");
    initSyncMarkDone(INIT_ETHERNET_DONE, false, INIT_ETHERNET_OK);
    vTaskDelete(NULL);
    return;
  }
  
  Serial1.println("- [*] Ethernet initialized successfully.");
  Serial1.print("      IP Address: ");
  Serial1.println(Ethernet.localIP());
  Serial1.print("      Hostname: ");
  Serial1.print(MDNS_NAME);
  Serial1.println(" (.local)");
  
  initSyncMarkDone(INIT_ETHERNET_DONE, true, INIT_ETHERNET_OK);
  vTaskDelete(NULL);

}