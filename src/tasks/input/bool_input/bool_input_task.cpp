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
    pinMode(INPUT_1_PIN, INPUT);
    pinMode(INPUT_2_PIN, INPUT);
    pinMode(INPUT_3_PIN, INPUT);
    pinMode(INPUT_4_PIN, INPUT);
    pinMode(INPUT_5_PIN, INPUT);
    pinMode(INPUT_6_PIN, INPUT);
    
    while (1) {

        // Current ms
        unsigned long currentMillis = millis();

        // Print the button state to Serial
        // Serial1.print("[");
        // Serial1.print(currentMillis);
        // // Serial1.print("] Button State: ");
        // Serial1.print(digitalRead(INPUT_1_PIN));
        // Serial1.print(" ");
        // Serial1.print(digitalRead(INPUT_2_PIN));
        // Serial1.print(" ");
        // Serial1.print(digitalRead(INPUT_3_PIN));
        // Serial1.print(" ");
        // Serial1.print(digitalRead(INPUT_4_PIN));
        // Serial1.print(" ");
        // Serial1.print(digitalRead(INPUT_5_PIN));
        // Serial1.print(" ");
        // Serial1.println(digitalRead(INPUT_6_PIN));


        // Delay for a short period to debounce and avoid flooding the serial output
        vTaskDelay(pdMS_TO_TICKS(100));

    }
}