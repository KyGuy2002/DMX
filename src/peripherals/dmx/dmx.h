#include <Arduino.h>
#include "DmxOutput.h"
#include "pins.h"
#include "settings.h"
#include "../../init/sync.h"


extern DmxOutput dmxOutput;
extern uint8_t dmxBuffer[512 + 1];
extern SemaphoreHandle_t xDmxMutex;


extern void createDmxInitTask();

void dmxInitTask(void *pvParameters);