// PIPES FINAL

#include <Arduino.h>
#include "DmxInput.h"
#include <Adafruit_NeoPixel.h>
#include <iostream>
#include <algorithm>

void neopixelGlowWireTick(int startChannel, int noPixels, int& ticksSinceLastGlowWireUpdate, int& currentPix);

#define PIN_TOP_NEOS 14  // GPIO pin connected
#define PIN_LONG_NEO_2 13  // GPIO pin connected
#define PIN_EXTRA_NEO_1 12  // GPIO pin connected
#define PIN_TALL_POST_NEO 11  // GPIO pin connected
#define PIN_LONG_NEO_3 10  // GPIO pin connected
#define PIN_LONG_NEO_1 9  // GPIO pin connected
#define PIN_EXTRA_NEO_2 8  // GPIO pin connected

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

// #define TOP_MAX_PIXELS 100
// #define LONG_MAX_PIXELS 200
// #define EXTRA_1_MAX_PIXELS 200
// #define EXTRA_2_MAX_PIXELS 80
// #define TALL_POST_MAX_PIXELS 150


#define START_CHANNEL 79
#define NUM_CHANNELS 200

DmxInput dmxInput;

int LED_COUNT = 200;
Adafruit_NeoPixel long1Strip(LED_COUNT, PIN_LONG_NEO_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel long2Strip(LED_COUNT, PIN_LONG_NEO_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel long3Strip(LED_COUNT, PIN_LONG_NEO_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel extra1Strip(LED_COUNT, PIN_EXTRA_NEO_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel extra2Strip(LED_COUNT, PIN_EXTRA_NEO_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tallPostStrip(LED_COUNT, PIN_TALL_POST_NEO, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel topStrip(LED_COUNT, PIN_TOP_NEOS, NEO_GRB + NEO_KHZ800);

volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS + START_CHANNEL)];
// Create a shifted view so dmx[0] corresponds to DMX channel START_CHANNEL
volatile uint8_t* dmx = buffer + START_CHANNEL - 1;

// Choose a human-invisible PWM frequency
const uint32_t PWM_HZ = 5000;      // 5 kHz

// Core 1 function for NeoPixel operations
void setup1() {


    // Init NeoPixels
    long1Strip.begin();
    long1Strip.show();
    long1Strip.setBrightness(255);

    long2Strip.begin();
    long2Strip.show();
    long2Strip.setBrightness(255);

    long3Strip.begin();
    long3Strip.show();
    long3Strip.setBrightness(255);

    extra1Strip.begin();
    extra1Strip.show();
    extra1Strip.setBrightness(255);

    extra2Strip.begin();
    extra2Strip.show();
    extra2Strip.setBrightness(255);

    tallPostStrip.begin();
    tallPostStrip.show();
    tallPostStrip.setBrightness(255);

    topStrip.begin();
    topStrip.show();
    topStrip.setBrightness(255);

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

}


int ticksSinceLastGlowWireUpdateTopStrip = 0;
int currentPixTopStrip = 0;
// int ticksSinceLastGlowWireUpdateLong2Strip = 0;
// int currentPixLong2Strip = 0;
// int ticksSinceLastGlowWireUpdateExtra1Strip = 0;
// int currentPixExtra1Strip = 0;
// int ticksSinceLastGlowWireUpdateTallPostStrip = 0;
// int currentPixTallPostStrip = 0;
// int ticksSinceLastGlowWireUpdateLong3Strip = 0;
// int currentPixLong3Strip = 0;
// int ticksSinceLastGlowWireUpdateLong1Strip = 0;
// int currentPixLong1Strip = 0;
// int ticksSinceLastGlowWireUpdateExtra2Strip = 0;
// int currentPixExtra2Strip = 0;

void loop1() {

    // Basic stuff
    digitalWrite(PIN_REL_1, (dmx[1] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_2, (dmx[2] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_3, (dmx[3] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_4, (dmx[4] > 127) ? LOW : HIGH);
    digitalWrite(PIN_REL_5, (dmx[5] > 127) ? LOW : HIGH);

    analogWrite(PIN_DIM_1, dmx[6]);
    analogWrite(PIN_DIM_2, dmx[7]);
    analogWrite(PIN_DIM_3, dmx[8]);
    analogWrite(PIN_DIM_4, dmx[9]);
    analogWrite(PIN_DIM_5, dmx[10]);
    analogWrite(PIN_DIM_6, dmx[11]);
    analogWrite(PIN_DIM_7, dmx[12]);
    analogWrite(PIN_DIM_8, dmx[13]);

    neopixelGlowWireTick(14, 200, ticksSinceLastGlowWireUpdateTopStrip, currentPixTopStrip); // Pipe
    // neopixelGlowWireTick(14, long2Strip, LONG_MAX_PIXELS, ticksSinceLastGlowWireUpdateLong2Strip, currentPixLong2Strip); // Door A
    // neopixelGlowWireTick(14, extra1Strip, EXTRA_1_MAX_PIXELS, ticksSinceLastGlowWireUpdateExtra1Strip, currentPixExtra1Strip); // Door B
    // neopixelGlowWireTick(14, tallPostStrip, TALL_POST_MAX_PIXELS, ticksSinceLastGlowWireUpdateTallPostStrip, currentPixTallPostStrip); // Door B
    // neopixelGlowWireTick(14, long3Strip, LONG_MAX_PIXELS, ticksSinceLastGlowWireUpdateLong3Strip, currentPixLong3Strip); // Door B
    // neopixelGlowWireTick(14, long1Strip, LONG_MAX_PIXELS, ticksSinceLastGlowWireUpdateLong1Strip, currentPixLong1Strip); // Door B
    // neopixelGlowWireTick(14, extra2Strip, EXTRA_2_MAX_PIXELS, ticksSinceLastGlowWireUpdateExtra2Strip, currentPixExtra2Strip); // Door B

    delayMicroseconds(1);
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
    dmxInput.begin(PIN_DMX, START_CHANNEL, NUM_CHANNELS + START_CHANNEL - 1);
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
    for (uint i = 0; i < 10; i++)
    {
        Serial1.print(dmx[i]);
        Serial1.print(", ");
    }
    Serial1.println("");
}




// Neopixel Glow Wire func
void neopixelGlowWireTick(int startChannel, int noPixels, int& ticksSinceLastGlowWireUpdate, int& currentPix) {

    // Speed
    // int ticksBetween = ((1 - (dmx[startChannel + 3] / 255.0f)) * 5) + 0.01; // Speed from 0 ticks to 15 ticks between updates
    int ticksBetween = 100;
    if (ticksSinceLastGlowWireUpdate < ticksBetween) {
        ticksSinceLastGlowWireUpdate++;
        return;
    }
    ticksSinceLastGlowWireUpdate = 0;

    // Read params
    int dotRed = dmx[startChannel];
    int dotGreen = dmx[startChannel + 1];
    int dotBlue = dmx[startChannel + 2];
    int dotCount = ((dmx[startChannel + 4] / 255.0f) * 19) + 1; // Ensure at least 1 dot
    int backRed = dmx[startChannel + 5];
    int backGreen = dmx[startChannel + 6];
    int backBlue = dmx[startChannel + 7];

    const int spacing = (200 / dotCount);

    // Set all background color
    for (uint16_t i = 0; i < 200; i++) {
        topStrip.setPixelColor(i, topStrip.Color(backRed, backGreen, backBlue));
        long2Strip.setPixelColor(i, long2Strip.Color(backRed, backGreen, backBlue));
        extra1Strip.setPixelColor(i, extra1Strip.Color(backRed, backGreen, backBlue));
        tallPostStrip.setPixelColor(i, tallPostStrip.Color(backRed, backGreen, backBlue));
        long3Strip.setPixelColor(i, long3Strip.Color(backRed, backGreen, backBlue));
        long1Strip.setPixelColor(i, long1Strip.Color(backRed, backGreen, backBlue));
        extra2Strip.setPixelColor(i, extra2Strip.Color(backRed, backGreen,  backBlue));
    }

    for (uint16_t i = 0; i < dotCount; i++) {
        int pix = (currentPix + i * spacing) % 200;
        topStrip.setPixelColor(pix, topStrip.Color(dotRed, dotGreen, dotBlue));
        long2Strip.setPixelColor(pix, long2Strip.Color(dotRed, dotGreen, dotBlue));
        extra1Strip.setPixelColor(pix, extra1Strip.Color(dotRed, dotGreen, dotBlue));
        tallPostStrip.setPixelColor(pix, tallPostStrip.Color(dotRed, dotGreen, dotBlue));
        long3Strip.setPixelColor(pix, long3Strip.Color(dotRed, dotGreen, dotBlue));
        long1Strip.setPixelColor(pix, long1Strip.Color(dotRed, dotGreen, dotBlue));
        extra2Strip.setPixelColor(pix, extra2Strip.Color(dotRed, dotGreen, dotBlue));
    }

    if (currentPix++ >= spacing) {
        currentPix = 0;
    }

    topStrip.show();
    long2Strip.show();
    extra1Strip.show();
    tallPostStrip.show();
    long3Strip.show();
    long1Strip.show();
    extra2Strip.show();

}