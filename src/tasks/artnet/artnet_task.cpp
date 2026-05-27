#include "artnet_task.h"

#include "../../peripherals/ethernet/ethernet.h"


Artnet artnet;



void createArtnetTask() {


  char SERVICE_NAME[30];
  snprintf(SERVICE_NAME, sizeof(SERVICE_NAME), "ProjectDMX %02x%02x", MAC_ADDRESS[4], MAC_ADDRESS[5]);

  artnet.setArtPollReplyConfigLongName(SERVICE_NAME);
  artnet.setArtPollReplyConfigShortName(SERVICE_NAME);

  // Artnet shares the same Ethernet controller as web/mDNS; protect init with mutex.
  if (xSemaphoreTake(xEthernetMutex, portMAX_DELAY) != pdTRUE) {
    Serial1.println("[Artnet Task] Failed to take Ethernet mutex during initialization!");
    vTaskDelete(NULL);
    return;
  }

  artnet.begin();
  xSemaphoreGive(xEthernetMutex);

  for (uint16_t universe = 0; universe < UNIVERSE_COUNT; ++universe) {
    artnet.subscribeArtDmxUniverse(universe, artnetCallback);
  }


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

    if (xSemaphoreTake(xEthernetMutex, portMAX_DELAY) != pdTRUE) {
      Serial1.println("[Artnet Task] Failed to take Ethernet mutex!");
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }

    artnet.parse();

    xSemaphoreGive(xEthernetMutex);

    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(20));
    
  }
}

void artnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {

  // Skip if mutex not available
  if (xSemaphoreTake(xDmxMutex, portMAX_DELAY) != pdTRUE) {
    vTaskDelay(pdMS_TO_TICKS(10));
    Serial1.println("==== Failed to take DMX mutex in Art-Net callback");
    return;
  }

  const uint16_t universeIndex = (static_cast<uint16_t>(metadata.net) << 8)
                               | (static_cast<uint16_t>(metadata.subnet) << 4)
                               | static_cast<uint16_t>(metadata.universe);

  if (universeIndex < UNIVERSE_COUNT) {
    memset(dmxBuffer[universeIndex], 0, sizeof(dmxBuffer[universeIndex]));
    const uint16_t copySize = size > 512 ? 512 : size;
    dmxBuffer[universeIndex][0] = 0; // DMX start code
    memcpy(dmxBuffer[universeIndex] + 1, data, copySize);
  }

  xSemaphoreGive(xDmxMutex);

}