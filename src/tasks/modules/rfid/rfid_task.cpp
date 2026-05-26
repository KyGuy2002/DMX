#include "rfid_task.h"


// Wiegand Settings
#define WIEGAND_TIMEOUT_MS 50

#define WIEGAND_D0_BIT 8
#define WIEGAND_D1_BIT 9

// Volatile variables used in the ISR
volatile uint32_t card_buffer = 0;
volatile uint8_t  bit_count = 0;
volatile uint32_t last_bit_time = 0;
volatile bool     packet_ready = false;

Adafruit_MCP23X17 mcp;


void createRfidTask() {

  Serial1.println("===== Rfid task created. - start");

  Wire1.setSCL(MODULE_A_PIN_2);
  Wire1.setSDA(MODULE_A_PIN_3);

  // // Maximize I2C clock speed to minimize read latency
  // // Wire1.setClock(400000);

  Serial1.println("=== RFID INIT START");
  
  if (!mcp.begin_I2C(0x20, &Wire1)) {
    while (1) { 
      Serial1.println("Error: Could not find MCP23017.");
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }

  Serial1.println("=== success!");

  // Serial1.println("===== RFID GOOD!!! ======");

  // // Configure Wiegand Pins as Inputs with Pull-ups enabled
  // mcp.pinMode(WIEGAND_D1_BIT, INPUT_PULLUP);
  // mcp.pinMode(WIEGAND_D0_BIT, INPUT_PULLUP);

  // // Change mirroring: INT Pins are separated (INTA tracks Port A)
  // // Int Out pin: Active LOW (FALLING edge)
  // mcp.setupInterrupts(true, false, LOW);

  // // Enable interrupt tracking on Wiegand pins on CHANGE
  // mcp.setupInterruptPin(WIEGAND_D1_BIT, CHANGE);
  // mcp.setupInterruptPin(WIEGAND_D0_BIT, CHANGE);

  // Serial1.println("===== c");

  // // Attach native RP2350 hardware interrupt to INTA pin
  // pinMode(MODULE_B_PIN_1, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(MODULE_B_PIN_1), handleMcpInterrupt, FALLING);

  // Serial1.println("===== d");

  // xTaskCreate(
  //   rfidTask,        // Task function
  //   "Rfid",         // Task name
  //   WEB_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
  //   NULL,                     // Parameters
  //   1,      // Priority
  //   NULL
  // );
  
}


void rfidTask(void *pvParameters) {
  while(1) {
    // Non-blocking timeout detection to process complete frames
    if (bit_count > 0 && (millis() - last_bit_time > WIEGAND_TIMEOUT_MS)) {
        
        // Critical Section: Copy data and clear buffers safely
        noInterrupts();
        uint32_t final_card = card_buffer;
        uint8_t final_bits = bit_count;
        card_buffer = 0;
        bit_count = 0;
        interrupts();

        // Decode packet processing
        if (final_bits == 26) {
            uint8_t facilityCode = (final_card >> 17) & 0xFF;
            uint16_t cardNumber = (final_card >> 1) & 0xFFFF;
            
            Serial.printf("Wiegand 26 Detected!\n");
            Serial.printf("Facility Code: %d\n", facilityCode);
            Serial.printf("Card Number:   %d\n", cardNumber);
            Serial.printf("Raw Hex:       0x%X\n\n", final_card);
        } else {
            Serial.printf("Unknown Format: %d bits captured. Raw: 0x%X\n\n", final_bits, final_card);
        }
    }

    vTaskDelay(pdMS_TO_TICKS(100)); // Polling delay to reduce CPU usage
  }
}


// Ultra-fast Hardware Interrupt Service Routine
__attribute__((section(".iram1"))) void handleMcpInterrupt() {
    // Read capturing registers to clear the interrupt flag condition on the hardware
    uint8_t pin_triggered = mcp.getLastInterruptPin();
    uint16_t pin_values = mcp.getCapturedInterrupt(); // Returns 16-bit state of both ports

    // Track falling edge state transitions for Port B pins
    if (pin_triggered == WIEGAND_D0_BIT && !(pin_values & (1 << WIEGAND_D0_BIT))) {
        card_buffer = (card_buffer << 1);
        bit_count++;
        last_bit_time = millis();
    } 
    else if (pin_triggered == WIEGAND_D1_BIT && !(pin_values & (1 << WIEGAND_D1_BIT))) {
        card_buffer = (card_buffer << 1) | 1;
        bit_count++;
        last_bit_time = millis();
    }
}