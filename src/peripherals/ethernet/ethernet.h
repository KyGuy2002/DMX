#include <Arduino.h>
#include <Ethernet3.h>
#include <ArduinoMDNS.h>
#include "pins.h"
#include "settings.h"
#include "../../init/sync.h"

extern void createEthernetInitTask();

void ethernetInitTask(void *pvParameters);