#include "sd.h"



void createSDInitTask() {
  xTaskCreate(
    sdInitTask,             // Task function
    "SD Init",              // Task name
    2048 / sizeof(StackType_t), // Stack size (words; 2048-byte intent)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void sdInitTask(void *pvParameters) {


  // SD uses SPI(0)
  SPI.setSCK(SD_SCK_PIN);
  SPI.setTX(SD_MOSI_PIN);
  SPI.setRX(SD_MISO_PIN);
  SPI.begin();


  if (!SD.begin(SD_CS_PIN, SPI)) {
    Serial1.println("- [X] SD init failed!");
    initSyncMarkDone(INIT_SD_DONE, false, INIT_SD_OK);
    vTaskDelete(NULL);
    return;
  }
  
  Serial1.println("- [*] SD initialized successfully.");
  initSyncMarkDone(INIT_SD_DONE, true, INIT_SD_OK);
  vTaskDelete(NULL);

}