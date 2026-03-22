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

void sendButtonPressHttpRequest() {
    // Try to acquire Ethernet mutex but don't block forever
    if (xSemaphoreTake(xEthernetMutex, pdMS_TO_TICKS(200)) != pdTRUE) {
        Serial1.println("[Bool Input] Ethernet mutex unavailable, aborting HTTP request");
        return;
    }
    
    // Check if the target device is available
    if (!isTargetDeviceAvailable) {
        Serial1.println("[Bool Input] Target device not available yet");
        xSemaphoreGive(xEthernetMutex);
        return;
    }
    
    Serial1.println("[Bool Input] Connecting to pdmx-controller.local");
    
    // Create a client to send HTTP request
    EthernetClient client;
    
    // Set an extremely short timeout (200ms)
    client.setTimeout(200);
    
    // Attempt connection to target device using hostname
    unsigned long startTime = millis();
    bool connected = client.connect("pdmx-controller.local", 80);
    unsigned long connectTime = millis() - startTime;
    
    if (connected && connectTime < 500) {
        Serial1.println("[Bool Input] Connected to server, sending request...");
        
        // Send HTTP GET request
        client.println("GET /button-press HTTP/1.1");
        client.println("Host: pdmx-controller.local");
        client.println("Connection: close");
        client.println();
        
        // Yield to allow other tasks to run during response wait
        vTaskDelay(pdMS_TO_TICKS(10));
        
        // Wait for response with yield points
        bool receivedResponse = false;
        unsigned long timeout = millis() + 500; // 500ms max for response
        while (client.connected() && millis() < timeout) {
            if (client.available()) {
                char c = client.read();
                Serial1.write(c);
                receivedResponse = true;
            }
            // Yield frequently to prevent starving other tasks
            vTaskDelay(pdMS_TO_TICKS(5));
        }
        
        client.stop();
        Serial1.println();
        
        if (receivedResponse) {
            Serial1.println("[Bool Input] Request successful - received response");
        } else {
            Serial1.println("[Bool Input] Connection made but no response received");
        }
    } else {
        Serial1.print("[Bool Input] Failed to connect (took ");
        Serial1.print(connectTime);
        Serial1.println("ms)");
        client.stop();
    }
    
    // Release the Ethernet mutex
    xSemaphoreGive(xEthernetMutex);
}

// Task to handle HTTP request in background (separate from button input)
void httpRequestTask(void *pvParameters) {
    sendButtonPressHttpRequest();
    // Delete this temporary task when done
    vTaskDelete(NULL);
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
                xTaskCreate(
                    httpRequestTask,
                    "HTTP Request",
                    2048 / sizeof(StackType_t),  // Smaller stack needed
                    NULL,
                    0,  // Idle priority - doesn't interfere with other tasks
                    NULL
                );
                // Button task immediately returns to waiting for next press
                vTaskDelay(pdMS_TO_TICKS(1));  // Yield to scheduler
            }
        }
    }
}