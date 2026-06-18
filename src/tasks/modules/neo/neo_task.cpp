#include "neo_task.h"


Adafruit_NeoPixel strip1(NEO_LENGTH, MODULE_C_PIN_1, NEO_GRB + NEO_KHZ800);


void createNeoTask() {

  Serial1.println("Neo task created.");

  strip1.begin();
  strip1.show();
  strip1.setBrightness(100);


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

    Serial1.println(dmxFrameSnapshot[0]);
    // Write Neopixel data
    for (int i = 0; i < NEO_LENGTH; i++) {
      uint16_t idx = i * 3;
      strip1.setPixelColor(i, dmxFrameSnapshot[idx], dmxFrameSnapshot[idx + 1], dmxFrameSnapshot[idx + 2]);
    }

    strip1.show();


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(20));
    
  }
}