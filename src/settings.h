#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

// Audio settings
static const char *MP3_PATH = "/test.mp3";

// Network settings
static byte MAC_ADDRESS[] = { 0x02, 0x3C, 0x2B, 0xCF, 0x07, 0xDF };

static const int UNIVERSE_COUNT = 10; // Number of DMX universes to support

static const int NEO_LENGTH = 170; // Number of neopixels in the strip

#endif // SETTINGS_H