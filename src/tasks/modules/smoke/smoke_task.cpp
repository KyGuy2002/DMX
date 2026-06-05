#include "smoke_task.h"




void createSmokeTask() {

  Serial1.println("Smoke task created.");

  pinMode(MODULE_A_PIN_1, INPUT);
  pinMode(MODULE_A_PIN_2, OUTPUT);


  xTaskCreate(
    smokeTask,        // Task function
    "Smoke",         // Task name
    WEB_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    1,      // Priority
    NULL
  );
  
}


void smokeTask(void *pvParameters) {
  static uint8_t dmxFrameSnapshot[512];

  while (1) {

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    memcpy(dmxFrameSnapshot, dmxBuffer[2], sizeof(dmxFrameSnapshot));
    xSemaphoreGive(xDmxMutex);

    

    digitalWrite(MODULE_A_PIN_2, dmxFrameSnapshot[4] > 127 ? HIGH : LOW);
    


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(20));
    
  }
}