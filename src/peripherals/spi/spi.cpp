#include "spi.h"


SemaphoreHandle_t xSPIMutex = xSemaphoreCreateMutex();


void createSPIInitTask() {
  xTaskCreate(
    spiInitTask,             // Task function
    "SPI Init",              // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void spiInitTask(void *pvParameters) {

  SPI.setSCK(SCK_PIN);
  SPI.setTX(MOSI_PIN);
  SPI.setRX(MISO_PIN);
  pinMode(ETH_CS_PIN, OUTPUT); // Ethernet
  pinMode(SD_CS_PIN, OUTPUT); // SD Card
  digitalWrite(ETH_CS_PIN, HIGH); // Ethernet
  digitalWrite(SD_CS_PIN, HIGH); // SD Card
  SPI.begin();


  Serial1.println("- [*] SPI initialized successfully.");
  initSyncMarkDone(INIT_SPI_DONE, true, INIT_SPI_OK);
  vTaskDelete(NULL);

}