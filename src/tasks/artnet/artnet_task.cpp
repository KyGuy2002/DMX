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
    1,      // Priority
    NULL
  );
  
}


void artnetTask(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(xEthernetMutex, pdMS_TO_TICKS(1)) == pdTRUE) {
      artnet.parse();
      xSemaphoreGive(xEthernetMutex);
    }

    taskYIELD();   // not 20ms delay
  }
}

void artnetCallback(const uint8_t *data, uint16_t size,
                    const ArtDmxMetadata &metadata,
                    const ArtNetRemoteInfo &remote) {

  if (metadata.universe >= UNIVERSE_COUNT) return;

  if (xSemaphoreTake(xDmxMutex, 0) != pdTRUE) {
    return;   // drop packet rather than blocking Art-Net receive
  }

  const uint16_t copySize = size > 512 ? 512 : size;

  memcpy(dmxBuffer[metadata.universe], data, copySize);

  if (copySize < 512) {
    memset(dmxBuffer[metadata.universe] + copySize, 0, 512 - copySize);
  }

  xSemaphoreGive(xDmxMutex);
}