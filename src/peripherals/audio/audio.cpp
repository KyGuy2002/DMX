#include "audio.h"


I2SStream i2s;
MP3DecoderHelix mp3;
VolumeStream volume(i2s);
EncodedAudioStream decoder(&volume, &mp3);



void createAudioInitTask() {
  xTaskCreate(
    audioInitTask,             // Task function
    "Audio Init",              // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void audioInitTask(void *pvParameters) {


  // Wait for SD init
  if (!initSyncWaitOk(INIT_SD_DONE, INIT_SD_OK)) {
    Serial1.println("- [X] Failed to initialize I2S: SD init timeout");
    initSyncMarkDone(INIT_AUDIO_DONE, false, INIT_AUDIO_OK);
    vTaskDelete(NULL);
    return;
  }


  auto config = i2s.defaultConfig(TX_MODE);
  config.sample_rate = 48000;
  config.bits_per_sample = 16;
  config.channels = 2;
  config.buffer_size = AUDIO_TASK_STACK_SIZE;
  config.pin_bck = AUDIO_BCK_PIN;
  config.pin_ws = AUDIO_LCK_PIN;
  config.pin_data = AUDIO_DIN_PIN;
  
  if (!i2s.begin(config)) {
    Serial1.println("- [X] Failed to initialize I2S");
    initSyncMarkDone(INIT_AUDIO_DONE, false, INIT_AUDIO_OK);
    vTaskDelete(NULL);
    return;
  }

  decoder.addNotifyAudioChange(i2s);
  volume.setVolume(DEFAULT_VOLUME);
  decoder.begin();
  
  Serial1.println("- [*] I2S Audio initialized successfully.");
  initSyncMarkDone(INIT_AUDIO_DONE, true, INIT_AUDIO_OK);
  vTaskDelete(NULL);

}