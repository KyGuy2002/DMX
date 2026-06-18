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
        2,                        // Priority
        NULL                      // Task handle
    );
}


void dmxInitTask(void *pvParameters) {
    pinMode(DMX_MODE_PIN, OUTPUT);


    if (INPUT_MODE == "XLR") {
        digitalWrite(DMX_MODE_PIN, LOW); // Set to input mode
        dmxInput.begin(DMX_RX_PIN, 0, 512);
        dmxInput.read_async(dmxBuffer[0]);
    }
    else if (INPUT_MODE == "NET") {
        digitalWrite(DMX_MODE_PIN, HIGH); // Set to output mode
        dmxOutput.begin(DMX_TX_PIN);
    }

    Serial1.print("- [*] DMX ");
    Serial1.print(INPUT_MODE == "XLR" ? "Input" : "Output");
    Serial1.println(" initialized successfully.");
    
    initSyncMarkDone(INIT_DMX_DONE, true, INIT_DMX_OK);
    vTaskDelete(NULL);

}