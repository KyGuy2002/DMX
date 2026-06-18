#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#define FASTLED_INTERNAL
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include "../../../rtos_config.h"

#include "../../../peripherals/dmx/dmx.h"


extern void createNeoGenTask();

void neoGenTask(void *pvParameters);

void tickChase(int startChannel, int noPixels, int& ticksSinceLastGlowWireUpdate, int& currentPix, uint8_t dmx[512]);