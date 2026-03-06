#ifndef PERIPHERAL_INIT_H
#define PERIPHERAL_INIT_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet3.h>
#include <ArduinoMDNS.h>
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
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

// Initialization functions
bool initSPI();
bool initEthernet();
bool initSDCard();
bool initI2SAudio();

#endif // PERIPHERAL_INIT_H
