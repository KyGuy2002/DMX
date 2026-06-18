#include <Arduino.h>
#include "init.h"
#include "sync.h"

#include "../peripherals/oled/oled.h"
#include "../peripherals/ethernet/ethernet.h"
#include "../peripherals/sd/sd.h"
#include "../peripherals/audio/audio.h"
#include "../peripherals/dmx/dmx.h"

#include "../peripherals/oled/oled_loading_task.h"

#include "../tasks/oled/oled_task.h"
#include "../tasks/oled/oled_error_task.h"
#include "../tasks/audio/music_task.h"
#include "../tasks/web/web_task.h"
#include "../tasks/mdns/mdns_task.h"
#include "../tasks/artnet/artnet_task.h"
#include "../tasks/dmx_tx/dmx_tx_task.h"
#include "../tasks/modules/neo/neo_task.h"
#include "../tasks/modules/fet/fet_task.h"
#include "../tasks/input/bool_input/bool_input_task.h"
#include "../tasks/modules/rfid/rfid_task.h"
#include "../tasks/modules/smoke/smoke_task.h"

void createWatchdogTask();
void watchdogTask(void *pvParameters);



void initPeripherals() {
  Serial1.println("Initializing peripherals...");
  initSyncCreate();


  pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);

  createWatchdogTask();

  // Order/dependancies handled inside each init func
  createOLEDInitTask();
  createSDInitTask();
  // createAudioInitTask();
  if (INPUT_MODE == "NET") createEthernetInitTask();
  createDmxInitTask(); // Handles tx and rx setup
}


void startRegularTasks() {
  vTaskDelete(g_oledStartupSplashTaskHandle);

  Serial1.println("====Initialization Complete.");
  createOLEDTask();
  // createMusicTask();
  if (INPUT_MODE == "NET") createWebTask();
  if (INPUT_MODE == "NET") createMdnsTask();
  if (INPUT_MODE == "NET") createArtnetTask();
  if (INPUT_MODE == "NET") createDmxTxTask(); // Dmx Init handles input already
  // createBoolInputTask();
  // createNeoTask(); // Module C
  createFetTask(); // Module D
  // createRfidTask(); // Module B
  // createSmokeTask(); // Module A


}

void createWatchdogTask() {
  xTaskCreate(
    watchdogTask,             // Task function
    "Watchdog",              // Task name
    1024 / sizeof(StackType_t), // Stack size (words; 1024-byte intent)
    NULL,                     // Parameters
    4,                        // Priority
    NULL                      // Task handle
  );
}


void watchdogTask(void *pvParameters) {
  while (1) {
    

    // TODO more error handling + retry
    // Ethernet Error
    if (INPUT_MODE == "NET" && initSyncDoneError(INIT_ETHERNET_DONE, INIT_ETHERNET_OK)) {
      vTaskDelete(g_oledStartupSplashTaskHandle);
      createOLEDErrorTask((char*)"Startup Failed", (char*)"Network Error");
      vTaskDelete(NULL);
      return;
    }

    // Send success message if everything is done and successful
    if (
      // initSyncDoneOk(INIT_AUDIO_DONE, INIT_AUDIO_OK) &&
      initSyncDoneOk(INIT_OLED_DONE, INIT_OLED_OK) &&
      (INPUT_MODE != "NET" || initSyncDoneOk(INIT_ETHERNET_DONE, INIT_ETHERNET_OK)) &&
      initSyncDoneOk(INIT_SD_DONE, INIT_SD_OK) &&
      initSyncDoneOk(INIT_DMX_DONE, INIT_DMX_OK)
    ) {

      startRegularTasks();
      vTaskDelete(NULL);
      return;

    }


    vTaskDelay(pdMS_TO_TICKS(100));
  }
}