#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Artnet.h>
#include "../../rtos_config.h"

#include "../../peripherals/ethernet/ethernet.h"

extern ArtnetReceiver artnet;

extern void createArtnetTask();

void artnetTask(void *pvParameters);