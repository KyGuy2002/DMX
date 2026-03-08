#include "sd.h"



void createSDInitTask() {
  xTaskCreate(
    sdInitTask,             // Task function
    "SD Init",              // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void sdInitTask(void *pvParameters) {


  // Wait for SPI init
  if (!initSyncWaitOk(INIT_SPI_DONE, INIT_SPI_OK)) {
    Serial1.println("- [X] Failed to initialize SD: SPI init timeout");
    initSyncMarkDone(INIT_SD_DONE, false, INIT_SD_OK);
    vTaskDelete(NULL);
    return;
  }


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