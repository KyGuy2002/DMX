#include "task_audio.h"
#include "../peripherals/peripheral_init.h"
#include "../settings.h"
#include "../rtos_config.h"
#include "AudioTools.h"
#include <SD.h>

using namespace audio_tools;

TaskHandle_t audioTaskHandle = NULL;
static File audioFile;
static StreamCopy copier(decoder, audioFile, AUDIO_TASK_STACK_SIZE);
static bool audioInitialized = false;
static bool audioPlaying = false;

void audioPlaybackTask(void *pvParameters) {
  Serial1.println("[Audio Task] Started");
  
  // Small delay to ensure scheduler is fully running
  vTaskDelay(pdMS_TO_TICKS(100));


  if(xSemaphoreTake(xSPIMutex, portMAX_DELAY) != pdTRUE) {
    Serial1.println("[Audio Task] Failed to take SPI mutex during initialization!");
    vTaskDelete(NULL);
    return;
  }

  
  // Open MP3 file
  Serial1.println("[Audio Task] Opening MP3 file: " + String(MP3_PATH));
  audioFile = SD.open(MP3_PATH, FILE_READ);
  
  if (!audioFile) {
    Serial1.println("[Audio Task] Failed to open MP3 file!");
    vTaskDelete(NULL);
    return;
  }
  
  Serial1.println("[Audio Task] MP3 file opened successfully.");
  
  // Initialize copier
  copier.begin();
  
  audioInitialized = true;
  audioPlaying = true;
  Serial1.println("[Audio Task] Audio playback initialized and started.");

  xSemaphoreGive(xSPIMutex);
  
  while (1) {

    if (audioPlaying && audioInitialized) {


      // Skip if mutex not available
    if(xSemaphoreTake(xSPIMutex, portMAX_DELAY) != pdTRUE) vTaskDelay(pdMS_TO_TICKS(10));

      // Copy audio data
      if (!copier.copy()) {
        Serial1.println("[Audio Task] Playback complete. Rewinding...");
        audioFile.seek(0);
        copier.begin();
        vTaskDelay(pdMS_TO_TICKS(200));
      }

      // Give mutex back
      xSemaphoreGive(xSPIMutex);

      // Yield to allow system responsiveness
      taskYIELD();
    } else {
      // If not playing, give other tasks time
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void createAudioTask() {
  xTaskCreate(
    audioPlaybackTask,        // Task function
    "Audio Playback",         // Task name
    AUDIO_TASK_STACK_SIZE,    // Stack size (bytes) - larger for audio processing
    NULL,                     // Parameters
    AUDIO_TASK_PRIORITY,      // Priority (higher for audio)
    &audioTaskHandle          // Task handle
  );
}

void setAudioVolume(float vol) {
  volume.setVolume(vol);
}

void playAudioFile(const char *filepath) {
  if (audioFile) {
    audioFile.close();
  }
  
  audioFile = SD.open(filepath, FILE_READ);
  if (audioFile) {
    copier.begin();
    audioPlaying = true;
    Serial1.println("[Audio Task] Playing: " + String(filepath));
  } else {
    Serial1.println("[Audio Task] Failed to open: " + String(filepath));
  }
}

void stopAudio() {
  audioPlaying = false;
  Serial1.println("[Audio Task] Stopped");
}
