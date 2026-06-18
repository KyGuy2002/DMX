#include <Arduino.h>
#include "DmxOutput.h"
#include "DmxInput.h"
#include "pins.h"
#include "settings.h"
#include "../../init/sync.h"


extern DmxOutput dmxOutput;
extern DmxInput dmxInput;
extern uint8_t dmxBuffer[UNIVERSE_COUNT][512];
extern SemaphoreHandle_t xDmxMutex;


extern void createDmxInitTask();

void dmxInitTask(void *pvParameters);