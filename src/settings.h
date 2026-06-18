#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

// Audio settings
static const char *MP3_PATH = "/test.mp3";

// Network settings
static byte MAC_ADDRESS[] = { 0x02, 0x3C, 0x2B, 0xCF, 0x07, 0xDF };

static const int UNIVERSE_COUNT = 10; // Number of DMX universes to support


// NOTE: MUST BE IN UNIVERSE ORDER AND CONSECUTIVE
static const int NEO_A_LENGTH = 100;
static const int NEO_A_START_CHANNEL = 0;
static const int NEO_A_START_UNIVERSE = 2;

static const int NEO_B_LENGTH = 100;
static const int NEO_B_START_CHANNEL = 0;
static const int NEO_B_START_UNIVERSE = 4;

static const int NEO_C_LENGTH = 100;
static const int NEO_C_START_CHANNEL = 0;
static const int NEO_C_START_UNIVERSE = 6;

static const int NEO_D_LENGTH = 100;
static const int NEO_D_START_CHANNEL = 0;
static const int NEO_D_START_UNIVERSE = 8;

#endif // SETTINGS_H


/**
 * XLR DMX Out universe 0
 * Module D dimmer fet universe 1
 * Module C neopixel:
 * - Out 1: universe 2 and 3
 * - Out 2: universe 4 and 5
 * - Out 3: universe 6 and 7
 * - Out 4: universe 8 and 9
 */