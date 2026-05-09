#include "bool_input_task.h"
#include "../../../tasks/mdns/mdns_task.h"


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
    // Initialize button pin as input with pull-up
    pinMode(INPUT_1_PIN, INPUT_PULLUP);
    
    while (1) {

        // Current ms
        unsigned long currentMillis = millis();

        // Print the button state to Serial
        Serial1.print("[");
        Serial1.print(currentMillis);
        Serial1.print("] Button State: ");
        Serial1.println(analogRead(INPUT_1_PIN));


        // Delay for a short period to debounce and avoid flooding the serial output
        vTaskDelay(pdMS_TO_TICKS(100));

    }
}