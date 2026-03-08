#include <Arduino.h>
#include "init.h"
#include "sync.h"
#include "queue.h"

#include "../peripherals/oled/oled.h"
#include "../peripherals/spi/spi.h"
#include "../peripherals/ethernet/ethernet.h"
#include "../peripherals/sd/sd.h"
#include "../peripherals/audio/audio.h"

QueueHandle_t g_oledQueue = nullptr;

void createWatchdogTask();
void watchdogTask(void *pvParameters);



void initPeripherals() {
  Serial1.println("Initializing peripherals...");
  initSyncCreate();


  pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);


  g_oledQueue = xQueueCreate(8, sizeof(OledMsg));

  createWatchdogTask();

  // Order/dependancies handled inside each init func
  createOLEDInitTask();
  createSPIInitTask();
  createEthernetInitTask();
  createSDInitTask();
  createAudioInitTask();


}


void createWatchdogTask() {
  xTaskCreate(
    watchdogTask,             // Task function
    "Watchdog",              // Task name
    1024,                     // Stack size (bytes)
    NULL,                     // Parameters
    4,                        // Priority
    NULL                      // Task handle
  );
}


void watchdogTask(void *pvParameters) {
  while (1) {
    

    // Ethernet Error
    if (initSyncDoneError(INIT_ETHERNET_DONE, INIT_ETHERNET_OK)) {

      OledMsg msg{};
      strncpy(msg.title, "Startup Failed", sizeof(msg.title) - 1);
      msg.title[sizeof(msg.title) - 1] = '\0';
      strncpy(msg.desc, "Network Error", sizeof(msg.desc) - 1);
      msg.desc[sizeof(msg.desc) - 1] = '\0';

      xQueueSend(g_oledQueue, &msg, pdMS_TO_TICKS(50)); // send to other task
      vTaskDelete(NULL);
      return;
    }

    // Send success message if everything is done and successful
    if (initSyncDoneOk(INIT_SPI_DONE, INIT_SPI_OK) &&
      initSyncDoneOk(INIT_AUDIO_DONE, INIT_AUDIO_OK) &&
      initSyncDoneOk(INIT_OLED_DONE, INIT_OLED_OK) &&
      initSyncDoneOk(INIT_ETHERNET_DONE, INIT_ETHERNET_OK) &&
      initSyncDoneOk(INIT_SD_DONE, INIT_SD_OK))
    {

      OledMsg msg{};
      strncpy(msg.title, "Startup Complete", sizeof(msg.title) - 1);
      msg.title[sizeof(msg.title) - 1] = '\0';

      xQueueSend(g_oledQueue, &msg, pdMS_TO_TICKS(50)); // send to other task
      vTaskDelete(NULL);
      return;
    }


    vTaskDelay(pdMS_TO_TICKS(100));
  }
}