#include <Arduino.h>
#include <SPI.h>
#include "pins.h"
#include "../../init/sync.h"

extern SemaphoreHandle_t xSPIMutex;

extern void createSPIInitTask();

void spiInitTask(void *pvParameters);