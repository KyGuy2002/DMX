#include "artnet_task.h"


Artnet artnet;



void createArtnetTask() {


  char SERVICE_NAME[30];
  snprintf(SERVICE_NAME, sizeof(SERVICE_NAME), "ProjectDMX %02x%02x", MAC_ADDRESS[4], MAC_ADDRESS[5]);

  artnet.setArtPollReplyConfigLongName(SERVICE_NAME);
  artnet.setArtPollReplyConfigShortName(SERVICE_NAME);
  artnet.begin();

  artnet.subscribeArtDmxUniverse(0, artnetCallback);


  Serial1.println("Artnet callback created.");

  xTaskCreate(
    artnetTask,        // Task function
    "ArtNet",         // Task name
    AUDIO_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    2,      // Priority
    NULL
  );
  
}


void artnetTask(void *pvParameters) {
  while (1) {


    artnet.parse();


    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(10));
    
  }
}

void artnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
  Serial1.println("Received Art-Net DMX data");

  // Skip if mutex not available
  if (xSemaphoreTake(xDmxMutex, portMAX_DELAY) != pdTRUE) {
    vTaskDelay(pdMS_TO_TICKS(10));
    Serial1.println("==== Failed to take DMX mutex in Art-Net callback");
    return;
  }

  // Shift DMX data by one position in the universe array
  dmxBuffer[0] = 0; // DMX start code
  memcpy(dmxBuffer + 1, data, size);

  xSemaphoreGive(xDmxMutex);

}