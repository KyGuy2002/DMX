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
  while (1) {

    // Skip if mutex not available
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }


    dmxOutput.write(dmxBuffer, 512);

    while (dmxOutput.busy()) {}

    xSemaphoreGive(xDmxMutex);


    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(1));
    
  }
}