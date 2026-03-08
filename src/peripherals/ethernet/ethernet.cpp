#include "ethernet.h"


EthernetUDP udp;
MDNS mdns(udp);
EthernetServer server(80);
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
  if (!initSyncWaitOk(INIT_SPI_DONE, INIT_SPI_OK)) {
    Serial1.println("- [X] Failed to initialize Ethernet: SPI init timeout");
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
  
  server.begin();
  if (!mdns.begin(Ethernet.localIP(), MDNS_NAME)) {
    Serial1.println("- [X] Failed to initialize Ethernet: mDNS responder init failed!");
    initSyncMarkDone(INIT_ETHERNET_DONE, false, INIT_ETHERNET_OK);
    vTaskDelete(NULL);
    return;
  }

  char SERVICE_NAME[30]; // Used for pretty mDNS service name like "ProjectDMX xxxx" (._http part is not shown on mDNS clients but required)
  snprintf(SERVICE_NAME, sizeof(SERVICE_NAME), "ProjectDMX %02x%02x._http", MAC_ADDRESS[4], MAC_ADDRESS[5]);
  Serial1.println(SERVICE_NAME);
  if (!mdns.addServiceRecord(SERVICE_NAME, 80, MDNSServiceTCP)) {
    Serial1.println("- [X] Failed to initialize Ethernet: mDNS HTTP service registration failed!");
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