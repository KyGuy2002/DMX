#include <Arduino.h>
#include <Ethernet3.h>
#include <ArduinoMDNS.h>
#include "pins.h"
#include "settings.h"
#include "../../init/sync.h"

extern EthernetUDP udp;
extern char MDNS_NAME[10];
extern SemaphoreHandle_t xEthernetMutex;

extern EthernetServer server;
extern MDNS mdns;

extern void createEthernetInitTask();

void ethernetInitTask(void *pvParameters);