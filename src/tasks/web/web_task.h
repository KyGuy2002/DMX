#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Ethernet3.h>
#include "../../rtos_config.h"

#include "../../peripherals/ethernet/ethernet.h"
#include "../../peripherals/audio/audio.h"

extern EthernetServer server;
extern void createWebTask();

void webTask(void *pvParameters);