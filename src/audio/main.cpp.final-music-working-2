#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

using namespace audio_tools;

const int SD_CS_PIN = 13; // Pin for SD card chip select
const int SD_MOSI_PIN = 15; // Pin for SD card MOSI
const int SD_MISO_PIN = 12; // Pin for SD card MISO
const int SD_CLK_PIN = 14; // Pin for SD card clock

// MP3 file path on SD
static const char *MP3_PATH = "/test.mp3";

// ---------- AudioTools objects ----------
I2SStream i2s;                                    // I2S sink
MP3DecoderHelix mp3;                              // Helix MP3 decoder
VolumeStream volume(i2s);
EncodedAudioStream decoder(&volume, &mp3);            // decoded PCM -> I2S
StreamCopy copier;                                // copies file -> decoder
File audioFile;

// Arduino Setup
void setup(void) {  
  // Open Serial 
  Serial1.begin(115200);
  delay(1500);


  //  ================ SD Card Initialization ================
  SPI1.setSCK(SD_CLK_PIN);  // CLK
  SPI1.setTX(SD_MOSI_PIN);  // MOSI
  SPI1.setRX(SD_MISO_PIN);  // MISO

  if (!SD.begin(SD_CS_PIN, SPI1)) {  // CS
    Serial1.println("SD init failed!");
    while (1) {
      Serial1.println("SD init failed!");
      delay(1000);  // wait for a second
    }
  }
  Serial1.println("initialization done.");
  // =====================================================


  audioFile = SD.open(MP3_PATH, FILE_READ);
  if (!audioFile) {
    Serial1.println("Failed to open MP3 file: " + String(MP3_PATH));
    while (true) delay(1000);
  }
  
  
  // =============== I2S Initialization ================
  Serial1.println("starting I2S...");
  I2SConfig config = i2s.defaultConfig(TX_MODE);
  config.sample_rate = 48000;
  config.bits_per_sample = 16;
  config.channels = 2;
  config.buffer_size = 2048;

  config.pin_bck = 6;
  config.pin_ws = 7;
  config.pin_data = 8;

  Serial1.printf("Free heap: %u\n", rp2040.getFreeHeap());
  if (!i2s.begin(config)) {
    Serial1.println("Failed to initialize I2S");
    while (true) delay(1000);
  }
  // =====================================================
  decoder.addNotifyAudioChange(i2s);

  // Decoder will drive audio format changes downstream
  volume.setVolume(0.5);
  decoder.begin();

  // Start streaming: SD file -> MP3 decoder -> I2S
  copier.begin(decoder, audioFile);

  Serial.println("Playing...");
}

// Arduino loop - copy sound to out 
void loop() {
  // copy() returns false when done or on error
  if (!copier.copy()) {
    Serial.println("Done (or copy error). Rewinding...");
    audioFile.seek(0);
    copier.begin(decoder, audioFile);
    delay(200);
  }
}