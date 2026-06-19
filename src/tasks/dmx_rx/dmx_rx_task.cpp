#include "dmx_rx_task.h"



void createDmxRxTask() {

	Serial1.println("DMX rx task created.");
  

	xTaskCreate(
		dmxRxTask,        // Task function
		"Dmx Rx",         // Task name
		AUDIO_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
		NULL,                     // Parameters
		3,      // Priority (higher for audio)
		NULL
	);
  
}


void dmxRxTask(void *pvParameters) {
	static uint8_t dmxRxFrame[512 + 1]; // DMX frame buffer (1 start code + 512 channels)

	while (1) {

		dmxInput.read(dmxRxFrame);

		// Keep the shared DMX buffer locked for the shortest possible window.
		if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
			vTaskDelay(pdMS_TO_TICKS(2));
			continue;
		}

		memcpy(dmxBuffer[0], &dmxRxFrame[1], 512);

		xSemaphoreGive(xDmxMutex);


		// Yield briefly before preparing the next frame.
		vTaskDelay(pdMS_TO_TICKS(1));
    
	}
}
