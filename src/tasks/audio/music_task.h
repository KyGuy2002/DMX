#include <SD.h>
#include <FreeRTOS.h>
#include <task.h>
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "../../peripherals/audio/audio.h"
#include "../../peripherals/sd/sd.h"


extern void createMusicTask();

void musicTask(void *pvParameters);