#include <Arduino.h>
#include <SD.h>
#include "pins.h"
#include "settings.h"
#include "../../init/sync.h"



extern void createSDInitTask();

void sdInitTask(void *pvParameters);