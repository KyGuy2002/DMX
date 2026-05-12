#include "neo_task.h"


Adafruit_NeoPixel strip1(50, MODULE_C_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(50, MODULE_C_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(50, MODULE_C_PIN_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4(50, MODULE_C_PIN_4, NEO_GRB + NEO_KHZ800);


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
  static uint8_t dmxFrameSnapshot[512];

  while (1) {

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    memcpy(dmxFrameSnapshot, dmxBuffer, sizeof(dmxFrameSnapshot));
    xSemaphoreGive(xDmxMutex);

    

    
    // Write Neopixel data
    for (int i = 0; i < 50; i++) {
      // uint16_t idx = i * 3;
      uint16_t idx = 0;
      strip1.setPixelColor(i, dmxFrameSnapshot[idx + 1], dmxFrameSnapshot[idx + 2], dmxFrameSnapshot[idx + 3]);
      strip2.setPixelColor(i, dmxFrameSnapshot[idx + 1], dmxFrameSnapshot[idx + 2], dmxFrameSnapshot[idx + 3]);
      strip3.setPixelColor(i, dmxFrameSnapshot[idx + 1], dmxFrameSnapshot[idx + 2], dmxFrameSnapshot[idx + 3]);
      strip4.setPixelColor(i, dmxFrameSnapshot[idx + 1], dmxFrameSnapshot[idx + 2], dmxFrameSnapshot[idx + 3]);
    }

    strip1.show();
    strip2.show();
    strip3.show();
    strip4.show();


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(20));
    
  }
}