#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include "../../../rtos_config.h"

#include "../../../peripherals/dmx/dmx.h"


extern void createFetTask();

void fetTask(void *pvParameters);