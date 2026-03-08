#include "music_task.h"

using namespace audio_tools;

static File audioFile;
static StreamCopy copier(decoder, audioFile, AUDIO_TASK_STACK_SIZE);


void createMusicTask() {
  


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

  xSemaphoreGive(xSPIMutex);
  

  xTaskCreate(
    musicTask,        // Task function
    "Music",         // Task name
    AUDIO_TASK_STACK_SIZE,    // Stack size (bytes) - larger for audio processing
    NULL,                     // Parameters
    AUDIO_TASK_PRIORITY,      // Priority (higher for audio)
    NULL
  );
  
}


void musicTask(void *pvParameters) {
  while (1) {

    // Skip if mutex not available
    if (xSemaphoreTake(xSPIMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }

    // Copy audio data
    if (!copier.copy()) {
      Serial1.println("[Audio Task] Playback complete. Rewinding...");
      audioFile.seek(0);
      copier.begin();
      vTaskDelay(pdMS_TO_TICKS(200));
    }

    // Give mutex back
    xSemaphoreGive(xSPIMutex);

    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(1));
    
  }
}