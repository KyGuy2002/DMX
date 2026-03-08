#ifndef PERIPHERAL_INIT_H
#define PERIPHERAL_INIT_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet3.h>
#include <ArduinoMDNS.h>
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include <U8g2lib.h>
// #include <MUIU8g2.h>
#include <Wire.h>
#include "../pins.h"
#include "../settings.h"

using namespace audio_tools;

// Mutex
extern SemaphoreHandle_t xSPIMutex;

// Global peripheral objects
extern I2SStream i2s;
extern MP3DecoderHelix mp3;
extern VolumeStream volume;
extern EncodedAudioStream decoder;
extern EthernetUDP udp;
extern MDNS mdns;
extern EthernetServer server;
extern char MDNS_NAME[10];
extern U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2;
// extern MUIU8G2 mui;

// Initialization functions
bool initSPI();
bool initEthernet();
bool initSDCard();
bool initI2SAudio();
bool initOLED();

#endif // PERIPHERAL_INIT_H
