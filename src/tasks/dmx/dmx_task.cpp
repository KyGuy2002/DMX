#include "dmx_task.h"



void createDmxTask() {

  Serial1.println("DMX task created.");
  

  xTaskCreate(
    dmxTask,        // Task function
    "Dmx",         // Task name
    AUDIO_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    2,      // Priority (higher for audio)
    NULL
  );
  
}


void dmxTask(void *pvParameters) {
  static uint8_t dmxTxFrame[512 + 1]; // DMX frame buffer (1 start code + 512 channels)

  while (1) {

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    

    dmxTxFrame[0] = 0;
    memcpy(&dmxTxFrame[1], dmxBuffer[0], 512 + 1);

    xSemaphoreGive(xDmxMutex);

    dmxOutput.write(dmxTxFrame, 512 + 1);

    while (dmxOutput.busy()) {
      // Cooperative wait so lower-priority tasks can run while transmission is in flight.
      vTaskDelay(pdMS_TO_TICKS(1));
    }


    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(1));
    
  }
}