#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include "../../../rtos_config.h"
#include "../../../pins.h"

#include <Wiegand.h>


extern void createRfidTask();

void rfidTask(void *pvParameters);