#include "dmx.h"

DmxOutput dmxOutput;


void createDmxInitTask() {
    xTaskCreate(
        dmxInitTask,             // Task function
        "Dmx Init",              // Task name
        2048 / sizeof(StackType_t), // Stack size (words; 2048-byte intent)
        NULL,                     // Parameters
        3,                        // Priority
        NULL                      // Task handle
    );
}


void dmxInitTask(void *pvParameters) {


    dmxOutput.begin(DMX_TX_PIN);

  
    Serial1.println("- [*] DMX output initialized successfully.");
    
    initSyncMarkDone(INIT_DMX_DONE, true, INIT_DMX_OK);
    vTaskDelete(NULL);

}