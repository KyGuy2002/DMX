#include "fet_task.h"




void createFetTask() {

  Serial1.println("Fet task created.");

  pinMode(MODULE_D_PIN_1, OUTPUT);
  pinMode(MODULE_D_PIN_2, OUTPUT);
  pinMode(MODULE_D_PIN_3, OUTPUT);
  pinMode(MODULE_D_PIN_4, OUTPUT);


  xTaskCreate(
    fetTask,        // Task function
    "Fet",         // Task name
    WEB_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    1,      // Priority
    NULL
  );
  
}


void fetTask(void *pvParameters) {
  static uint8_t dmxFrameSnapshot[512];

  while (1) {

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    memcpy(dmxFrameSnapshot, dmxBuffer, sizeof(dmxFrameSnapshot));
    xSemaphoreGive(xDmxMutex);

    

    analogWrite(MODULE_D_PIN_1, dmxFrameSnapshot[1]);
    analogWrite(MODULE_D_PIN_2, dmxFrameSnapshot[2]);
    analogWrite(MODULE_D_PIN_3, dmxFrameSnapshot[3]);
    analogWrite(MODULE_D_PIN_4, dmxFrameSnapshot[4]);
    


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(20));
    
  }
}