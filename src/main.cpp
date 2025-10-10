#include <Arduino.h>
#include "DmxInput.h"
#include <Adafruit_NeoPixel.h>

#define STRIP_PIN 16   // GPIO pin connected
#define STRIP_PIN2 17  // GPIO pin connected
#define STRIP_PIN3 18  // GPIO pin connected
#define STRIP_PIN4 19  // GPIO pin connected

#define PIN_REL_1 2  // GPIO pin connected to relay 1
#define PIN_REL_2 3  // GPIO pin connected to relay 2
#define PIN_REL_3 4  // GPIO pin connected to relay 3
#define PIN_REL_4 5  // GPIO pin connected to relay 4
#define PIN_REL_5 6  // GPIO pin connected to relay 5
#define PIN_REL_6 7  // GPIO pin connected to relay 6
#define PIN_REL_7 22 // GPIO pin connected to relay 7

#define PIN_DIM_1 8  // GPIO pin connected to dimmer 1
#define PIN_DIM_2 9  // GPIO pin connected to dimmer 2
#define PIN_DIM_3 10 // GPIO pin connected to dimmer 3
#define PIN_DIM_4 11 // GPIO pin connected to dimmer 4
#define PIN_DIM_5 12 // GPIO pin connected to dimmer 5
#define PIN_DIM_6 13 // GPIO pin connected to dimmer 6
#define PIN_DIM_7 14 // GPIO pin connected to dimmer 7

#define PIN_DMX 15   // GPIO pin connected to DMX input

#define LED_COUNT 150

#define START_CHANNEL 1
#define NUM_CHANNELS 40

DmxInput dmxInput;

Adafruit_NeoPixel strip(LED_COUNT, STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, STRIP_PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3(LED_COUNT, STRIP_PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4(LED_COUNT, STRIP_PIN4, NEO_GRB + NEO_KHZ800);

volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];

// Choose a human-invisible PWM frequency
const uint32_t PWM_HZ = 5000;      // 5 kHz

// Core 1 function for NeoPixel operations
void setup1() {
    // Initialize NeoPixel strip on Core 1
    strip.begin();
    strip.show();
    strip.setBrightness(255);

    strip2.begin();
    strip2.show();
    strip2.setBrightness(255);

    strip3.begin();
    strip3.show();
    strip3.setBrightness(255);

    strip4.begin();
    strip4.show();
    strip4.setBrightness(255);

    buffer[0] = 0; // Start code

    buffer[1] = 50; // REL_1 Air Solenoid Relay
    buffer[2] = 50; // REL_2 Bottom Bolt Solenoid Relay
    buffer[3] = 50; // REL_3 Top Bolt Solenoid Relay
    buffer[4] = 50; // REL_4 Door Red Flasher Relay
    buffer[5] = 50; // REL_5 Spare Relay
    buffer[6] = 50; // REL_6 Spare Relay
    buffer[7] = 50; // REL_7 Spare Relay

    buffer[8] = 50;  // DIM_1 Right Blue Fiber Dimmer
    buffer[9] = 50;  // DIM_2 Top COB Glow Wire Dimmer
    buffer[10] = 50; // DIM_3 Left Red Fiber Dimmer
    buffer[11] = 50; // DIM_4 Door Gauge Dimmer
    buffer[12] = 50; // DIM_5 Door Indicator Group A Dimmer
    buffer[13] = 50; // DIM_6 Door Indicator Group B Dimmer
    buffer[14] = 50; // DIM_7 Door Indicator Group C Dimmer + Door COB Glow Wire Dimmer

    buffer[15] = 50;
    buffer[16] = 50;
    buffer[17] = 50;
    buffer[18] = 255;
    buffer[19] = 255;
    buffer[20] = 50;
    buffer[21] = 50;
    buffer[22] = 50;
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

    analogWrite(PIN_DIM_1, buffer[8]);
    analogWrite(PIN_DIM_2, buffer[9]);
    analogWrite(PIN_DIM_3, buffer[10]);
    analogWrite(PIN_DIM_4, buffer[11]);
    analogWrite(PIN_DIM_5, buffer[12]);
    analogWrite(PIN_DIM_6, buffer[13]);
    analogWrite(PIN_DIM_7, buffer[14]);

    // Neopixel stuff

    // Get params
    int dotRed = buffer[15];
    int dotGreen = buffer[16];
    int dotBlue = buffer[17];
    int speed = ((1 - (buffer[18] / 255.0f)) * 10) + 0.01; // Speed from 1 (fast) to 10 (slow)
    int dotCount = ((buffer[19] / 255.0f) * 19) + 1; // Ensure at least 1 dot
    int backRed = buffer[20];
    int backGreen = buffer[21];
    int backBlue = buffer[22];

    const int spacing = (strip.numPixels() / dotCount);

    // Set all background color
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(backRed, backGreen, backBlue));
        strip2.setPixelColor(i, strip2.Color(backRed, backGreen, backBlue));
        strip3.setPixelColor(i, strip3.Color(backRed, backGreen, backBlue));
        strip4.setPixelColor(i, strip4.Color(backRed, backGreen, backBlue));
    }

    for (uint16_t i = 0; i < dotCount; i++) {
        int pix = (currentPix + i * spacing) % strip.numPixels();
        strip.setPixelColor(pix, strip.Color(dotRed, dotGreen, dotBlue));
        strip2.setPixelColor(pix, strip2.Color(dotRed, dotGreen, dotBlue));
        strip3.setPixelColor(pix, strip3.Color(dotRed, dotGreen, dotBlue));
        strip4.setPixelColor(pix, strip4.Color(dotRed, dotGreen, dotBlue));
    }

    if (currentPix++ >= spacing) {
        currentPix = 0;
    }

    strip.show();
    strip2.show();
    strip3.show();
    strip4.show();
    delay(speed);
    
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
