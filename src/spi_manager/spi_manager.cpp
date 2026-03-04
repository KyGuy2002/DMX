#include "spi_manager.h"

SemaphoreHandle_t spiMutex = NULL;

void initSpiMutex() {
  if (spiMutex == NULL) {
    spiMutex = xSemaphoreCreateMutex();
    if (spiMutex == NULL) {
      Serial1.println("ERROR: Failed to create SPI mutex!");
    } else {
      Serial1.println("- [*] SPI Mutex created successfully.");
    }
  }
}

bool acquireSpiMutex() {
  if (spiMutex == NULL) {
    Serial1.println("ERROR: SPI mutex not initialized!");
    return false;
  }
  
  if (xSemaphoreTake(spiMutex, portMAX_DELAY) == pdTRUE) {
    return true;
  }
  
  return false;
}

bool acquireSpiMutexTimed(TickType_t timeoutTicks) {
  if (spiMutex == NULL) {
    Serial1.println("ERROR: SPI mutex not initialized!");
    return false;
  }
  
  if (xSemaphoreTake(spiMutex, timeoutTicks) == pdTRUE) {
    return true;
  }
  
  return false;
}

void releaseSpiMutex() {
  if (spiMutex != NULL) {
    xSemaphoreGive(spiMutex);
  }
}
