#include "bool_input_task.h"
#include "../../../tasks/mdns/mdns_task.h"

// Debounce settings
static const unsigned long DEBOUNCE_DELAY = 50; // ms
static volatile unsigned long lastInterruptTime = 0;

// Queue for button press events (using enum for event type)
enum ButtonEvent {
    BUTTON_PRESSED = 1
};

static QueueHandle_t buttonEventQueue = NULL;

void createBoolInputTask() {
    // Create the event queue before starting the task
    buttonEventQueue = xQueueCreate(5, sizeof(ButtonEvent));
    
    xTaskCreate(
        boolInputTask,             // Task function
        "Bool Input Task",    // Task name
        4096 / sizeof(StackType_t), // Stack size (words; increased for HTTP operations)
        NULL,                     // Parameters
        2,                        // Priority
        NULL
    );
}

// Interrupt Service Routine for button press
void buttonPressISR() {
    unsigned long currentTime = millis();
    
    // Debounce: ignore interrupts within DEBOUNCE_DELAY ms of last interrupt
    if ((currentTime - lastInterruptTime) > DEBOUNCE_DELAY) {
        lastInterruptTime = currentTime;
        
        // Queue the button press event
        ButtonEvent event = BUTTON_PRESSED;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(buttonEventQueue, &event, &xHigherPriorityTaskWoken);
        
        // Yield if a higher priority task was woken
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}


void boolInputTask(void *pvParameters) {
    // Initialize button pin as input with pull-up
    pinMode(INPUT_1_PIN, INPUT_PULLUP);
    
    // Attach interrupt to button pin (FALLING = pin goes to GND when pressed)
    attachInterrupt(digitalPinToInterrupt(INPUT_1_PIN), buttonPressISR, FALLING);
    
    Serial1.println("[Bool Input] Task started - interrupt driven");
    
    // Wait for button press events from the queue
    ButtonEvent event;
    while (1) {
        // Wait indefinitely for an event from the queue
        if (xQueueReceive(buttonEventQueue, &event, portMAX_DELAY) == pdTRUE) {
            if (event == BUTTON_PRESSED) {
                Serial1.println("[Bool Input] INPUT_1_PIN button pressed!");
                
                // Spawn a temporary background task for HTTP request
                // This prevents the button task from blocking other tasks
                
                // Button task immediately returns to waiting for next press
                vTaskDelay(pdMS_TO_TICKS(1));  // Yield to scheduler
            }
        }
    }
}