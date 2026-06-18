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

    const int universe = (INPUT_MODE == "NET") ? 1 : 0;
    const int chStart = (INPUT_MODE == "NET") ? 0 : 7; // Temp make room for 6 channel rockwedge light

    memcpy(dmxFrameSnapshot, dmxBuffer[universe], 512);
    xSemaphoreGive(xDmxMutex);

    // Serial1.print("DMX Snapshot:");
    // for (int i = 0; i < 20; i++) {
    //   char buf[4];
    //   // format as 3 characters with leading spaces
    //   snprintf(buf, sizeof(buf), "%3d", dmxFrameSnapshot[i]);
    //   Serial1.print(" ");
    //   Serial1.print(buf);
    // }
    // Serial1.println();

    

    analogWrite(MODULE_D_PIN_4, dmxFrameSnapshot[chStart]);
    analogWrite(MODULE_D_PIN_3, dmxFrameSnapshot[chStart + 1]);
    analogWrite(MODULE_D_PIN_2, dmxFrameSnapshot[chStart + 2]);
    analogWrite(MODULE_D_PIN_1, dmxFrameSnapshot[chStart + 3]);
    


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(20));
    
  }
}