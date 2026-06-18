#include "neo_task.h"


CRGB leds1[NEO_A_LENGTH];
CRGB leds2[NEO_B_LENGTH];
CRGB leds3[NEO_C_LENGTH];
CRGB leds4[NEO_D_LENGTH];


void createNeoTask() {

  Serial1.println("Neo task created.");

  FastLED.addLeds<WS2812B, MODULE_C_PIN_3, GRB>(leds1, NEO_A_LENGTH);
  FastLED.addLeds<WS2812B, MODULE_C_PIN_4, GRB>(leds2, NEO_B_LENGTH);
  FastLED.addLeds<WS2812B, MODULE_C_PIN_2, GRB>(leds3, NEO_C_LENGTH);
  FastLED.addLeds<WS2812B, MODULE_C_PIN_1, GRB>(leds4, NEO_D_LENGTH);
  FastLED.setBrightness(255);


  xTaskCreate(
    neoTask,        // Task function
    "Neo",         // Task name
    WEB_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    1,      // Priority
    NULL
  );
  
}


void neoTask(void *pvParameters) {
  static uint8_t dmxFrameSnapshot[8][512];

  while (1) {

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    // Copy relevant DMX universes
    for (int i = 0; i < 8; i++) {
      memcpy(dmxFrameSnapshot[i], dmxBuffer[NEO_A_START_UNIVERSE + i], 512);
    }
    xSemaphoreGive(xDmxMutex);

    writeStrip(leds1, NEO_A_START_UNIVERSE, NEO_A_LENGTH, dmxFrameSnapshot);
    writeStrip(leds2, NEO_B_START_UNIVERSE, NEO_B_LENGTH, dmxFrameSnapshot);
    writeStrip(leds3, NEO_C_START_UNIVERSE, NEO_C_LENGTH, dmxFrameSnapshot);
    writeStrip(leds4, NEO_D_START_UNIVERSE, NEO_D_LENGTH, dmxFrameSnapshot);

    FastLED.show();


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(10));
    
  }
}


void writeStrip(CRGB *strip, int startUniverse, int length, uint8_t dmxFrameSnapshot[8][512]) {

  // Write Neopixel data
  for (int i = 0; i < length; i++) {
    int universe = startUniverse;
    uint16_t idx = i * 3;

    if (idx >= 512) {
      universe = startUniverse + 1;
      idx = idx - 512;
    }

    strip[i].r = dmxFrameSnapshot[universe][idx++];
    strip[i].g = dmxFrameSnapshot[universe][idx++];
    strip[i].b = dmxFrameSnapshot[universe][idx++];
  }

}