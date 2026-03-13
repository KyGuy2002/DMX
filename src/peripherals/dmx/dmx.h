#include <Arduino.h>
#include "DmxOutput.h"
#include "pins.h"
#include "settings.h"
#include "../../init/sync.h"


extern DmxOutput dmxOutput;


extern void createDmxInitTask();

void dmxInitTask(void *pvParameters);