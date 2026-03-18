#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Adafruit_NeoPixel.h>
#include "../../../rtos_config.h"

#include "../../../peripherals/dmx/dmx.h"


extern void createNeoTask();

void neoTask(void *pvParameters);