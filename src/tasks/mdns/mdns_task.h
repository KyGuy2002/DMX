#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include <MDNS.h>
#include <Ethernet3.h>
#include "../../rtos_config.h"

#include "../../peripherals/ethernet/ethernet.h"

extern MDNS mdns;
extern void createMdnsTask();

void mdnsTask(void *pvParameters);