#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#define FASTLED_INTERNAL
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include "../../../rtos_config.h"

#include "../../../peripherals/dmx/dmx.h"


extern void createNeoTask();

void neoTask(void *pvParameters);

void writeStrip(CRGB *strip, int startUniverse, int length, uint8_t dmxFrameSnapshot[512]);