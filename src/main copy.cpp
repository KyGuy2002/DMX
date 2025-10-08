#include <Arduino.h>
#include "DmxInput.h"
DmxInput dmxInput;

#define START_CHANNEL 1
#define NUM_CHANNELS 10

volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];


// Core 1 function for NeoPixel operations
void setup() {
    // Initialize NeoPixel strip on Core 1
    
    pinMode(2, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    // Setup our DMX Input to read on GPIO 15, from channel 1 to 3
    dmxInput.begin(15, START_CHANNEL, NUM_CHANNELS);
    dmxInput.read_async(buffer);

    Serial1.begin(115200);
    Serial1.println("DMX Input Example");

}

void loop() {

    // digitalWrite(2, HIGH);
    // delay(500);
    // digitalWrite(2, LOW);
    // delay(500);

    delay(30);

    if(millis() > 100+dmxInput.latest_packet_timestamp()) {
        Serial1.println("no data!");
        return;
    }
    // Print the DMX channels
    Serial1.print("Received packet: ");
    for (uint i = 0; i < sizeof(buffer); i++)
    {
        Serial1.print(buffer[i]);
        Serial1.print(", ");
    }
    Serial1.println("");

    // Blink the LED to indicate that a packet was received

    digitalWrite(2, (buffer[1] > 127) ? LOW : HIGH);
    analogWrite(8, buffer[2]);
    analogWrite(9, buffer[3]);
    analogWrite(10, buffer[4]);

}