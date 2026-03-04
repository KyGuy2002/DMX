#include <Arduino.h>
#include "pins.h"
#include "settings.h"

#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

using namespace audio_tools;


// ========== Audio Objects
I2SStream i2s;
MP3DecoderHelix mp3;
VolumeStream volume(i2s);
EncodedAudioStream decoder(&volume, &mp3);
StreamCopy copier;
File audioFile;


void setup(void) {  
  Serial1.begin(115200);
  Serial1.println("Starting ProjectDMX Controller...");



  // ========== SPI Init
  SPI.setSCK(SCK_PIN);
  SPI.setTX(MOSI_PIN);
  SPI.setRX(MISO_PIN);



  // ========== SD Card Initialization
  if (!SD.begin(SD_CS_PIN, SPI)) {
    Serial1.println("- [X] SD init failed!");
    while (true) delay(500);
  }
  Serial1.println("- [*] SD Card initialized successfully.");



  // ========== Open MP3 File
  audioFile = SD.open(MP3_PATH, FILE_READ);
  if (!audioFile) {
    Serial1.println("- [X] Failed to open MP3 file: " + String(MP3_PATH));
    while (true) delay(500);
  }
  Serial1.println("- [*] MP3 file opened successfully.");



  // ========== I2S Initialization
  I2SConfig config = i2s.defaultConfig(TX_MODE);
  config.sample_rate = 48000;
  config.bits_per_sample = 16;
  config.channels = 2;
  config.buffer_size = 2048;
  config.pin_bck = AUDIO_BCK_PIN;
  config.pin_ws = AUDIO_LCK_PIN;
  config.pin_data = AUDIO_DIN_PIN;
  if (!i2s.begin(config)) {
    Serial1.println("- [X] Failed to initialize I2S");
    while (true) delay(500);
  }
  decoder.addNotifyAudioChange(i2s);
  volume.setVolume(0.5);
  decoder.begin();
  copier.begin(decoder, audioFile);
  Serial1.println("- [*] I2S Audio initialized successfully.");

}



void loop() {
  if (!copier.copy()) {
    Serial.println("Done (or copy error). Rewinding...");
    audioFile.seek(0);
    copier.begin(decoder, audioFile);
    delay(200);
  }
}