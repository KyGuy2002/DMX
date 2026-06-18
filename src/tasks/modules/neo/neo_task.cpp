#include "neo_task.h"


Adafruit_NeoPixel strip1(NEO_A_LENGTH, MODULE_A_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(NEO_B_LENGTH, MODULE_B_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(NEO_C_LENGTH, MODULE_C_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4(NEO_D_LENGTH, MODULE_D_PIN_1, NEO_GRB + NEO_KHZ800);


void createNeoTask() {

  Serial1.println("Neo task created.");

  strip1.begin();
  strip1.show();
  strip1.setBrightness(100);

  strip2.begin();
  strip2.show();
  strip2.setBrightness(100);

  strip3.begin();
  strip3.show();
  strip3.setBrightness(100);

  strip4.begin();
  strip4.show();
  strip4.setBrightness(100);


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

    writeStrip(strip1, NEO_A_START_UNIVERSE, NEO_A_LENGTH, dmxFrameSnapshot);
    writeStrip(strip2, NEO_B_START_UNIVERSE, NEO_B_LENGTH, dmxFrameSnapshot);
    writeStrip(strip3, NEO_C_START_UNIVERSE, NEO_C_LENGTH, dmxFrameSnapshot);
    writeStrip(strip4, NEO_D_START_UNIVERSE, NEO_D_LENGTH, dmxFrameSnapshot);


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(1));
    
  }
}


void writeStrip(Adafruit_NeoPixel &strip, int startUniverse, int length, uint8_t dmxFrameSnapshot[8][512]) {

  // Write Neopixel data
  for (int i = 0; i < length; i++) {
    int universe = startUniverse;
    uint16_t idx = i * 3;

    if (idx >= 512) {
      universe = startUniverse + 1;
      idx = idx - 512;
    }

    strip.setPixelColor(i, dmxFrameSnapshot[universe][idx], dmxFrameSnapshot[universe][idx + 1], dmxFrameSnapshot[universe][idx + 2]);
  }

  strip.show();

}