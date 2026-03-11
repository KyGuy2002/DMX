#include "ethernet.h"


EthernetUDP udp;
char MDNS_NAME[10];
SemaphoreHandle_t xEthernetMutex = xSemaphoreCreateMutex();

EthernetServer server(80);
MDNS mdns(udp);


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


  // Eth uses SPI1
  SPI1.setSCK(ETH_SCK_PIN);
  SPI1.setTX(ETH_MOSI_PIN);
  SPI1.setRX(ETH_MISO_PIN);
  SPI1.begin();


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