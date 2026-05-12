#include "bool_input_task.h"
#include "../../../tasks/mdns/mdns_task.h"

static const int ANALOG_PRESS_THRESHOLD = 15;
static const unsigned long POLL_DELAY_MS = 10;

void createBoolInputTask() {
    xTaskCreate(
        boolInputTask,             // Task function
        "Bool Input Task",    // Task name
        4096 / sizeof(StackType_t), // Stack size (words; increased for HTTP operations)
        NULL,                     // Parameters
        2,                        // Priority
        NULL
    );
}


void boolInputTask(void *pvParameters) {
    pinMode(INPUT_1_PIN, INPUT);

    Serial1.println("[Bool Input] Task started - analog polling");

    bool wasPressed = false;
    
    while (1) {
        int analogValue = analogRead(INPUT_1_PIN);
        bool isPressed = analogValue < ANALOG_PRESS_THRESHOLD;

        if (isPressed && !wasPressed) {
            wasPressed = true;
            Serial1.println("[Bool Input] INPUT_1_PIN button pressed!");

            // Button task immediately returns to waiting for next press
            vTaskDelay(pdMS_TO_TICKS(1));
        } else if (!isPressed) {
            wasPressed = false;
        }

        vTaskDelay(pdMS_TO_TICKS(POLL_DELAY_MS));
    }
}