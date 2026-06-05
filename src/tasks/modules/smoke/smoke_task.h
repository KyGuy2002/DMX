#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include "../../../rtos_config.h"

#include "../../../peripherals/dmx/dmx.h"


extern void createSmokeTask();

void smokeTask(void *pvParameters);