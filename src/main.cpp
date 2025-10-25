// PIPES FINAL

#include <Arduino.h>
#include "DmxInput.h"
#include <iostream>
#include <algorithm>


#define PIN_REL_1 20  // GPIO pin connected to relay 1
#define PIN_REL_2 19  // GPIO pin connected to relay 2
#define PIN_REL_3 18  // GPIO pin connected to relay 3
#define PIN_REL_4 17  // GPIO pin connected to relay 4
#define PIN_REL_5 16  // GPIO pin connected to relay 5

#define PIN_DIM_1 2  // GPIO pin connected to dimmer 1
#define PIN_DIM_2 3  // GPIO pin connected to dimmer 2
#define PIN_DIM_3 4 // GPIO pin connected to dimmer 3
#define PIN_DIM_4 5 // GPIO pin connected to dimmer 4
#define PIN_DIM_5 6 // GPIO pin connected to dimmer 5
#define PIN_DIM_6 7 // GPIO pin connected to dimmer 6
#define PIN_DIM_7 22 // GPIO pin connected to dimmer 7
#define PIN_DIM_8 21 // GPIO pin connected to dimmer 8

#define PIN_DMX 15   // GPIO pin connected to DMX input



#define START_CHANNEL 29
#define NUM_CHANNELS 70

DmxInput dmxInput;


volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];
volatile uint8_t* dmx = buffer + START_CHANNEL - 1;  // so dmx[1] == channel 29

// Choose a human-invisible PWM frequency
const uint32_t PWM_HZ = 5000;      // 5 kHz

// Core 1 function for NeoPixel operations
void setup1() {

    // Default DMX values before controller is connected
    // Use the shifted view so dmx[0] == DMX channel START_CHANNEL (29)
    dmx[0] = 0; // Start code

    dmx[1] = 50; // REL_1 Air Solenoid Relay 1
    dmx[2] = 50; // REL_2 Air Solenoid Relay 2
    dmx[3] = 50; // REL_3 Air Solenoid Relay 3
    dmx[4] = 50; // REL_4 Fog Machine Relay
    dmx[5] = 50; // REL_5 Spare Relay

    dmx[6] = 50;  // DIM_1 Bistro - 5v
    dmx[7] = 50;  // DIM_2 Split Pole 2
    dmx[8] = 50;  // DIM_3 Tall Pole
    dmx[9] = 50;  // DIM_4 Tiny Pole + Split Pole 1
    dmx[10] = 50; // DIM_5 L Fiber
    dmx[11] = 50; // DIM_6 Left COB
    dmx[12] = 50; // DIM_7 R Fiber
    dmx[13] = 50; // DIM_8 TOP COB

    dmx[14] = 50; // Pipe Neo Dot Red
    dmx[15] = 50; // Pipe Neo Dot Green
    dmx[16] = 50; // Pipe Neo Dot Blue
    dmx[17] = 50; // Pipe Neo Dot Speed
    dmx[18] = 50; // Pipe Neo Dot Count
    dmx[19] = 50; // Pipe Neo Dot Background Red
    dmx[20] = 50; // Pipe Neo Dot Background Green
    dmx[21] = 50; // Pipe Neo Dot Background Blue

    dmx[22] = 50; // Door Neo A Dot Red
    dmx[23] = 50; // Door Neo A Dot Green
    dmx[24] = 50; // Door Neo A Dot Blue
    dmx[25] = 50; // Door Neo A Dot Speed
    dmx[26] = 50; // Door Neo A Dot Count
    dmx[27] = 50; // Door Neo A Dot Background Red
    dmx[28] = 50; // Door Neo A Dot Background Green
    dmx[29] = 50; // Door Neo A Dot Background Blue

    dmx[30] = 50; // Door Neo B Dot Red
    dmx[31] = 50; // Door Neo B Dot Green
    dmx[32] = 50; // Door Neo B Dot Blue
    dmx[33] = 50; // Door Neo B Dot Speed
    dmx[34] = 50; // Door Neo B Dot Count
    dmx[35] = 50; // Door Neo B Dot Background Red
    dmx[36] = 50; // Door Neo B Dot Background Green
    dmx[37] = 50; // Door Neo B Dot Background Blue

    dmx[38] = 50; // Door Neo B Dot Red
    dmx[39] = 50; // Door Neo B Dot Green
    dmx[40] = 50; // Door Neo B Dot Blue
    dmx[41] = 50; // Door Neo B Dot Speed
    dmx[42] = 50; // Door Neo B Dot Count
    dmx[43] = 50; // Door Neo B Dot Background Red
    dmx[44] = 50; // Door Neo B Dot Background Green
    dmx[45] = 50; // Door Neo B Dot Background Blue

    dmx[46] = 50; // Door Neo B Dot Red
    dmx[47] = 50; // Door Neo B Dot Green
    dmx[48] = 50; // Door Neo B Dot Blue
    dmx[49] = 50; // Door Neo B Dot Speed
    dmx[50] = 50; // Door Neo B Dot Count
    dmx[51] = 50; // Door Neo B Dot Background Red
    dmx[52] = 50; // Door Neo B Dot Background Green
    dmx[53] = 50; // Door Neo B Dot Background Blue

    dmx[54] = 50; // Door Neo B Dot Red
    dmx[55] = 50; // Door Neo B Dot Green
    dmx[56] = 50; // Door Neo B Dot Blue
    dmx[57] = 50; // Door Neo B Dot Speed
    dmx[58] = 50; // Door Neo B Dot Count
    dmx[59] = 50; // Door Neo B Dot Background Red
    dmx[60] = 50; // Door Neo B Dot Background Green
    dmx[61] = 50; // Door Neo B Dot Background Blue

    dmx[62] = 50; // Door Neo B Dot Red
    dmx[63] = 50; // Door Neo B Dot Green
    dmx[64] = 50; // Door Neo B Dot Blue
    dmx[65] = 50; // Door Neo B Dot Speed
    dmx[66] = 50; // Door Neo B Dot Count
    dmx[67] = 50; // Door Neo B Dot Background Red
    dmx[68] = 50; // Door Neo B Dot Background Green
    dmx[69] = 50; // Door Neo B Dot Background Blue

}



void loop1() {

    Serial1.println("Core 1 running loop1()");
    analogWrite(PIN_DIM_4, 255);

    // Basic stuff
    digitalWrite(PIN_REL_1, (dmx[1] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_2, (dmx[2] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_3, (dmx[3] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_4, (dmx[4] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_5, (dmx[5] > 127) ? LOW : HIGH);

    analogWrite(PIN_DIM_1, dmx[6]);
    analogWrite(PIN_DIM_2, dmx[7]);
    analogWrite(PIN_DIM_3, dmx[8]);
    // analogWrite(PIN_DIM_4, dmx[9]);
    analogWrite(PIN_DIM_5, dmx[10]);
    analogWrite(PIN_DIM_6, dmx[11]);
    analogWrite(PIN_DIM_7, dmx[12]);
    analogWrite(PIN_DIM_8, dmx[13]);

    delay(1);
}

void setup() {
    // Core 0: Handle DMX input and PWM output
    // (NeoPixel initialization moved to Core 1)

    pinMode(PIN_REL_1, OUTPUT);
    pinMode(PIN_REL_2, OUTPUT);
    pinMode(PIN_REL_3, OUTPUT);
    pinMode(PIN_REL_4, OUTPUT);
    pinMode(PIN_REL_5, OUTPUT);

    pinMode(PIN_DIM_1, OUTPUT);
    pinMode(PIN_DIM_2, OUTPUT);
    pinMode(PIN_DIM_3, OUTPUT);
    pinMode(PIN_DIM_4, OUTPUT);
    pinMode(PIN_DIM_5, OUTPUT);
    pinMode(PIN_DIM_6, OUTPUT);
    pinMode(PIN_DIM_7, OUTPUT);
    pinMode(PIN_DIM_8, OUTPUT);

    // Setup our DMX Input to read on GPIO 15, from channel 1 to 3
   dmxInput.begin(PIN_DMX, START_CHANNEL, NUM_CHANNELS); // COUNT, not end
  dmxInput.read_async((uint8_t*)buffer); 

    Serial1.begin(115200);
    delay(1000);
    Serial1.println("DMX Input Example - Multicore Version");

    analogWriteFreq(PWM_HZ);
    analogWriteRange(255);
}

void loop() {

}