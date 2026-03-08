#include <Arduino.h>
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "pins.h"
#include "../../settings.h"
#include "../../rtos_config.h"
#include "../../init/sync.h"

extern I2SStream i2s;
extern MP3DecoderHelix mp3;
extern VolumeStream volume;
extern EncodedAudioStream decoder;


extern void createAudioInitTask();

void audioInitTask(void *pvParameters);