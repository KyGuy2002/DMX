#ifndef TASK_AUDIO_H
#define TASK_AUDIO_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// Task handle
extern TaskHandle_t audioTaskHandle;

// Task function
void audioPlaybackTask(void *pvParameters);

// Task creation
void createAudioTask();

// Control functions
void setAudioVolume(float volume);
void playAudioFile(const char *filepath);
void stopAudio();

#endif // TASK_AUDIO_H
