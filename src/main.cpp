#include <Arduino.h>
#include "DmxInput.h"
#include <Adafruit_NeoPixel.h>
#include <iostream>
#include <algorithm>

void neopixelGlowWireTick(int startChannel, Adafruit_NeoPixel& strip, int noPixels);
void neopixelSparkRimTick(int startChannel, Adafruit_NeoPixel& strip, int noPixels);

#define PIN_PIPE_NEO 18   // GPIO pin connected
#define PIN_DOOR_A_NEO 17  // GPIO pin connected
#define PIN_DOOR_B_NEO 16  // GPIO pin connected
#define PIN_RIM_NEO 19  // GPIO pin connected

#define PIN_REL_1 2  // GPIO pin connected to relay 1
#define PIN_REL_2 3  // GPIO pin connected to relay 2
#define PIN_REL_3 4  // GPIO pin connected to relay 3
#define PIN_REL_4 5  // GPIO pin connected to relay 4
#define PIN_REL_5 6  // GPIO pin connected to relay 5
#define PIN_REL_6 7  // GPIO pin connected to relay 6
#define PIN_REL_7 22 // GPIO pin connected to relay 7
#define PIN_REL_8 21 // GPIO pin connected to relay 8

#define PIN_DIM_1 8  // GPIO pin connected to dimmer 1
#define PIN_DIM_2 9  // GPIO pin connected to dimmer 2
#define PIN_DIM_3 10 // GPIO pin connected to dimmer 3
#define PIN_DIM_4 11 // GPIO pin connected to dimmer 4
#define PIN_DIM_5 12 // GPIO pin connected to dimmer 5
#define PIN_DIM_6 13 // GPIO pin connected to dimmer 6
#define PIN_DIM_7 14 // GPIO pin connected to dimmer 7

#define PIN_DMX 15   // GPIO pin connected to DMX input

#define PIPE_MAX_PIXELS 100
#define DOOR_A_MAX_PIXELS 200
#define DOOR_B_MAX_PIXELS 200
#define RIM_MAX_PIXELS 80

#define START_CHANNEL 29
#define NUM_CHANNELS 45

DmxInput dmxInput;

int LED_COUNT = std::max({PIPE_MAX_PIXELS, DOOR_A_MAX_PIXELS, DOOR_B_MAX_PIXELS, RIM_MAX_PIXELS});
Adafruit_NeoPixel pipeStrip(LED_COUNT, PIN_PIPE_NEO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel doorAStrip(LED_COUNT, PIN_DOOR_A_NEO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel doorBStrip(LED_COUNT, PIN_DOOR_B_NEO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rimStrip(LED_COUNT, PIN_RIM_NEO, NEO_GRB + NEO_KHZ800);

volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];

// Choose a human-invisible PWM frequency
const uint32_t PWM_HZ = 5000;      // 5 kHz

// Core 1 function for NeoPixel operations
void setup1() {


    // Init NeoPixels
    pipeStrip.begin();
    pipeStrip.show();
    pipeStrip.setBrightness(255);

    doorAStrip.begin();
    doorAStrip.show();
    doorAStrip.setBrightness(255);

    doorBStrip.begin();
    doorBStrip.show();
    doorBStrip.setBrightness(255);

    rimStrip.begin();
    rimStrip.show();
    rimStrip.setBrightness(255);

    // Default DMX values before controller is connected
    buffer[0] = 0; // Start code

    buffer[1] = 50; // REL_1 Air Solenoid Relay
    buffer[2] = 50; // REL_2 Bottom Bolt Solenoid Relay
    buffer[3] = 50; // REL_3 Top Bolt Solenoid Relay
    buffer[4] = 50; // REL_4 Door Red Flasher Relay
    buffer[5] = 50; // REL_5 Fog Machine Relay
    buffer[6] = 50; // REL_6 Spare Relay
    buffer[7] = 50; // REL_7 Spare Relay
    buffer[8] = 50; // REL_8 Spare Relay

    buffer[9] = 50;  // DIM_1 Right Blue Fiber Dimmer
    buffer[10] = 50; // DIM_2 Top COB Glow Wire Dimmer
    buffer[11] = 50; // DIM_3 Left Red Fiber Dimmer
    buffer[12] = 50; // DIM_4 Door Gauge Dimmer
    buffer[13] = 50; // DIM_5 Door Indicator Group A Dimmer
    buffer[14] = 50; // DIM_6 Door Indicator Group B Dimmer
    buffer[15] = 50; // DIM_7 Door Indicator Group C Dimmer + Door COB Glow Wire Dimmer

    buffer[16] = 50; // Pipe Neo Dot Red
    buffer[17] = 50; // Pipe Neo Dot Green
    buffer[18] = 50; // Pipe Neo Dot Blue
    buffer[19] = 255; // Pipe Neo Dot Speed
    buffer[20] = 255; // Pipe Neo Dot Count
    buffer[21] = 50; // Pipe Neo Dot Background Red
    buffer[22] = 50; // Pipe Neo Dot Background Green
    buffer[23] = 50; // Pipe Neo Dot Background Blue

    buffer[24] = 50; // Door Neo A Dot Red
    buffer[25] = 50; // Door Neo A Dot Green
    buffer[26] = 50; // Door Neo A Dot Blue
    buffer[27] = 255; // Door Neo A Dot Speed
    buffer[28] = 255; // Door Neo A Dot Count
    buffer[29] = 50; // Door Neo A Dot Background Red
    buffer[30] = 50; // Door Neo A Dot Background Green
    buffer[31] = 50; // Door Neo A Dot Background Blue

    buffer[32] = 50; // Door Neo B Dot Red
    buffer[33] = 50; // Door Neo B Dot Green
    buffer[34] = 50; // Door Neo B Dot Blue
    buffer[35] = 255; // Door Neo B Dot Speed
    buffer[36] = 255; // Door Neo B Dot Count
    buffer[37] = 50; // Door Neo B Dot Background Red
    buffer[38] = 50; // Door Neo B Dot Background Green
    buffer[39] = 50; // Door Neo B Dot Background Blue


    buffer[40] = 50; // Rim Neo Spark Red
    buffer[41] = 50; // Rim Neo Spark Green
    buffer[42] = 50; // Rim Neo Spark Blue
    buffer[43] = 255; // Rim Neo Spark Count
    buffer[44] = 255; // Rim Neo Spark Duration

}


int currentPix = 0;

void loop1() {

    // Basic stuff
    digitalWrite(PIN_REL_1, (buffer[1] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_2, (buffer[2] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_3, (buffer[3] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_4, (buffer[4] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_5, (buffer[5] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_6, (buffer[6] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_7, (buffer[7] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_8, (buffer[8] > 127) ? LOW : HIGH);

    analogWrite(PIN_DIM_1, buffer[9]);
    analogWrite(PIN_DIM_2, buffer[10]);
    analogWrite(PIN_DIM_3, buffer[11]);
    analogWrite(PIN_DIM_4, buffer[12]);
    analogWrite(PIN_DIM_5, buffer[13]);
    analogWrite(PIN_DIM_6, buffer[14]);
    analogWrite(PIN_DIM_7, buffer[15]);

    neopixelGlowWireTick(16, pipeStrip, PIPE_MAX_PIXELS); // Pipe
    neopixelGlowWireTick(24, doorAStrip, DOOR_A_MAX_PIXELS); // Door A
    neopixelGlowWireTick(32, doorBStrip, DOOR_B_MAX_PIXELS); // Door B

    neopixelSparkRimTick(40, rimStrip, RIM_MAX_PIXELS); // Rim

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
    pinMode(PIN_REL_6, OUTPUT);
    pinMode(PIN_REL_7, OUTPUT);

    pinMode(PIN_DIM_1, OUTPUT);
    pinMode(PIN_DIM_2, OUTPUT);
    pinMode(PIN_DIM_3, OUTPUT);
    pinMode(PIN_DIM_4, OUTPUT);
    pinMode(PIN_DIM_5, OUTPUT);
    pinMode(PIN_DIM_6, OUTPUT);
    pinMode(PIN_DIM_7, OUTPUT);

    // Setup our DMX Input to read on GPIO 15, from channel 1 to 3
    dmxInput.begin(PIN_DMX, START_CHANNEL, NUM_CHANNELS);
    dmxInput.read_async(buffer);

    Serial1.begin(115200);
    delay(1000);
    Serial1.println("DMX Input Example - Multicore Version");

    analogWriteFreq(PWM_HZ);
    analogWriteRange(255);
}

void loop() {
    // Core 0: Handle DMX input and PWM output only
    
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
}




// Neopixel Glow Wire func
int ticksSinceLastGlowWireUpdate = 0;
void neopixelGlowWireTick(int startChannel, Adafruit_NeoPixel& strip, int noPixels) {

    // Speed
    int ticksBetween = ((1 - (buffer[startChannel + 3] / 255.0f)) * 15) + 0.01; // Speed from 0 ticks to 15 ticks between updates
    if (ticksSinceLastGlowWireUpdate < ticksBetween) {
        ticksSinceLastGlowWireUpdate++;
        return;
    }
    ticksSinceLastGlowWireUpdate = 0;

    // Read params
    int dotRed = buffer[startChannel];
    int dotGreen = buffer[startChannel + 1];
    int dotBlue = buffer[startChannel + 2];
    int dotCount = ((buffer[startChannel + 4] / 255.0f) * 19) + 1; // Ensure at least 1 dot
    int backRed = buffer[startChannel + 5];
    int backGreen = buffer[startChannel + 6];
    int backBlue = buffer[startChannel + 7];

    const int spacing = (strip.numPixels() / dotCount);

    // Set all background color
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(backRed, backGreen, backBlue));
    }

    for (uint16_t i = 0; i < dotCount; i++) {
        int pix = (currentPix + i * spacing) % strip.numPixels();
        strip.setPixelColor(pix, strip.Color(dotRed, dotGreen, dotBlue));
    }

    if (currentPix++ >= spacing) {
        currentPix = 0;
    }

    strip.show();

}


// Neopixel Spark Rim func
int ticksSinceLastSparkRimUpdate = 0;
void neopixelSparkRimTick(int startChannel, Adafruit_NeoPixel& strip, int noPixels) {

    // Read params
    int sparkRed = buffer[startChannel];
    int sparkGreen = buffer[startChannel + 1];
    int sparkBlue = buffer[startChannel + 2];
    int sparkCount = buffer[startChannel + 3];
    int sparkDuration = ((1 - (buffer[startChannel + 5] / 255.0f)) * 30) + 1; // 1 ticks to 30 ticks between despawn

    // Set all background color
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }

    for (int i = 0; i < sparkCount; i++) {
        int pix = random(0, strip.numPixels());
        strip.setPixelColor(pix, strip.Color(sparkRed, sparkGreen, sparkBlue));
    }

    strip.show();

}