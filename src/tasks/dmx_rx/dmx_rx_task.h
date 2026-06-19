#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>

#include "../../rtos_config.h"

#include "../../peripherals/dmx/dmx.h"


extern void createDmxRxTask();

void dmxRxTask(void *pvParameters);