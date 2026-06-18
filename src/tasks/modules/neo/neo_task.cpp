#include "neo_task.h"


CRGB leds[170];

void createNeoTask() {

  Serial1.println("Neo task created.");

  FastLED.addLeds<WS2812B, MODULE_C_PIN_1, GRB>(leds, 170);
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
  static uint8_t dmxFrameSnapshot[512];

  while (1) {

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    memcpy(dmxFrameSnapshot, dmxBuffer[1], sizeof(dmxFrameSnapshot));
    xSemaphoreGive(xDmxMutex);

    // Write Neopixel data
    for (int i = 0; i < 170; i++) {
      uint16_t idx = i * 3;
      leds[i].r = dmxFrameSnapshot[idx++];
      leds[i].g = dmxFrameSnapshot[idx++];
      leds[i].b = dmxFrameSnapshot[idx++];
    }

    
    FastLED.show();


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(20));
    
  }
}