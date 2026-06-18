#include "dmx.h"

DmxOutput dmxOutput;
DmxInput dmxInput;
uint8_t dmxBuffer[UNIVERSE_COUNT][512];
SemaphoreHandle_t xDmxMutex = xSemaphoreCreateMutex();


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


void dmxReadTask(void *pvParameters) {
    while (1) {
        dmxInput.read_async(dmxBuffer[0]);
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}


void dmxInitTask(void *pvParameters) {


    if (INPUT_MODE == "XLR") {
        dmxInput.begin(DMX_RX_PIN, 0, 512);

        xTaskCreate(
            dmxReadTask,
            "Dmx Read",
            2048 / sizeof(StackType_t),
            NULL,
            1,
            NULL
        );
    }
    else if (INPUT_MODE == "NET") {
        dmxOutput.begin(DMX_TX_PIN);
    }

    Serial1.print("- [*] DMX ");
    Serial1.print(INPUT_MODE == "XLR" ? "Input" : "Output");
    Serial1.println(" initialized successfully.");
    
    initSyncMarkDone(INIT_DMX_DONE, true, INIT_DMX_OK);
    vTaskDelete(NULL);

}