#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
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
EncodedAudioStream decoder(&i2s, &mp3);
StreamCopy copier;
File audioFile;

void myTask(void *pvParameters);


void setup(void) {  


  Serial1.begin(115200);
  Serial1.println();
  Serial1.println();
  Serial1.println("Starting ProjectDMX Controller...");
  


  // ========== SPI Init
  SPI.setSCK(SCK_PIN);
  SPI.setTX(MOSI_PIN);
  SPI.setRX(MISO_PIN);
  pinMode(ETH_CS_PIN, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(ETH_CS_PIN, HIGH);
  digitalWrite(SD_CS_PIN, HIGH);
  SPI.begin();



  // ========== SD Card Init
  if (!SD.begin(SD_CS_PIN, SPI)) {
    Serial1.println("- [X] SD init failed!");
    while (true) delay(1000);
  }
  Serial1.println("- [*] SD Card initialized successfully.");



  // ========== Open MP3 File
  audioFile = SD.open(MP3_PATH, FILE_READ);
  if (!audioFile) {
    Serial1.println("- [X] Failed to open MP3 file: " + String(MP3_PATH));
    while (true) delay(1000);
  }
  Serial1.println("- [*] MP3 file opened successfully.");



  // ========== I2S Audio Init
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
    while (true) delay(1000);
  }
  decoder.addNotifyAudioChange(i2s);
  decoder.begin();
  copier.begin(decoder, audioFile);
  Serial1.println("- [*] I2S Audio initialized successfully.");



  Serial1.println("Startup complete.");
  Serial1.println();
  Serial1.println();




  // Create the task
  xTaskCreate(
    myTask,        // Task function
    "MyTask",      // Task name
    2048 / 2,          // Stack size (words, not bytes)
    NULL,          // Parameters
    1,             // Priority
    NULL           // Task handle
  );

  Serial1.println("made task");

  // Start scheduler
  // vTaskStartScheduler();

  Serial1.println("no get here");



}


void myTask(void *pvParameters)
{
  for (;;) {
    if (!copier.copy()) {
      Serial1.println("Done (or copy error). Rewinding...");
      audioFile.seek(0);
      copier.begin(decoder, audioFile);
      vTaskDelay(pdMS_TO_TICKS(200));
    } else {
      vTaskDelay(1); // or taskYIELD();
    }
  }
}



void loop() {
  Serial1.println("main loop");
  vTaskDelay(pdMS_TO_TICKS(1000));
  // if (!copier.copy()) {
  //   Serial.println("Done (or copy error). Rewinding...");
  //   audioFile.seek(0);
  //   copier.begin(decoder, audioFile);
  //   delay(200);
  // }
}