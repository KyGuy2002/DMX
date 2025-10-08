#include <Arduino.h>
#include "DmxInput.h"
#include <Adafruit_NeoPixel.h>

#define STRIP_PIN 17  // GPIO pin connected
#define STRIP_PIN2 16  // GPIO pin connected

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 250
#define LED_START 2
DmxInput dmxInput;

#define START_CHANNEL 1
#define NUM_CHANNELS 10

Adafruit_NeoPixel strip(LED_COUNT, STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, STRIP_PIN2, NEO_GRB + NEO_KHZ800);

volatile uint8_t buffer[DMXINPUT_BUFFER_SIZE(START_CHANNEL, NUM_CHANNELS)];

// No shared variables needed - direct buffer access

// Board: Raspberry Pi Pico (RP2040) — Earle Philhower core
// MOSFET gate on GP15 (pin 20 on Pico)
const int LED_PIN = 15; 

// Choose a human-invisible PWM frequency
const uint32_t PWM_HZ = 5000;      // 5 kHz
const uint16_t PWM_RES = 65535;    // 16-bit range for smooth fades

// Core 1 function for NeoPixel operations
void setup1() {
    // Initialize NeoPixel strip on Core 1
    strip.begin();
    strip.show();
    strip.setBrightness(255);

    strip2.begin();
    strip2.show();
    strip2.setBrightness(255);

    buffer[0] = 0; // Start code
    buffer[1] = 255; // Brightness
    buffer[2] = 255; // Dot Red
    buffer[3] = 255; // Dot Green
    buffer[4] = 255; // Dot Blue
    buffer[5] = 255; // Speed
    buffer[6] = 255; // Dot Count
    buffer[7] = 255; // Back Red
    buffer[8] = 255; // Back Green
    buffer[9] = 255; // Back Blue
}


int currentPix = 0;

void loop1() {

    // Get params
    int dotRed = buffer[2];
    int dotGreen = buffer[3];
    int dotBlue = buffer[4];
    int speed = ((1 - (buffer[5] / 255.0f)) * 10) + 0.01; // Speed from 1 (fast) to 10 (slow)
    int dotCount = ((buffer[6] / 255.0f) * 19) + 1; // Ensure at least 1 dot
    int backRed = buffer[7];
    int backGreen = buffer[8];
    int backBlue = buffer[9];

    const int spacing = (strip.numPixels() / dotCount);

    // Set all background color
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(backRed, backGreen, backBlue));
        strip2.setPixelColor(i, strip2.Color(backRed, backGreen, backBlue));
    }

    for (uint16_t i = 0; i < dotCount; i++) {
        int pix = (currentPix + i * spacing) % strip.numPixels();
        strip.setPixelColor(pix, strip.Color(dotRed, dotGreen, dotBlue));
        strip2.setPixelColor(pix, strip2.Color(dotRed, dotGreen, dotBlue));
    }

    if (currentPix++ >= spacing) {
        currentPix = 0;
    }

    strip.show();
    strip2.show();
    delay(speed);
    
}

void setup() {
    // Core 0: Handle DMX input and PWM output
    // (NeoPixel initialization moved to Core 1)
    
    // Setup our DMX Input to read on GPIO 18, from channel 1 to 3
    dmxInput.begin(18, START_CHANNEL, NUM_CHANNELS);
    dmxInput.read_async(buffer);

    Serial1.begin(115200);
    delay(1000);
    Serial1.println("DMX Input Example - Multicore Version");

    pinMode(LED_PIN, OUTPUT);
    analogWriteFreq(PWM_HZ);
    analogWriteRange(PWM_RES);
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

    // PWM output for LED (non-blocking)
    uint16_t duty = (uint16_t)(PWM_RES * (buffer[1] / 255.0f));
    analogWrite(LED_PIN, duty);
    
    // Core 1 handles NeoPixel updates automatically
}
