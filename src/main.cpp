#include "AudioTools.h"

AudioInfo info(48000, 2, 16);
SineWaveGenerator<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
I2SStream out; 
StreamCopy copier(out, sound);                             // copies sound into i2s

// Arduino Setup
void setup(void) {  
  // Open Serial 
  Serial1.begin(115200);
  while(!Serial1);
  AudioToolsLogger.begin(Serial1, AudioToolsLogLevel::Info);

  // start I2S
  Serial1.println("starting I2S...");
  auto config = out.defaultConfig(TX_MODE);
  config.copyFrom(info); 
  out.begin(config);

  // Setup sine wave
  sineWave.begin(info, N_B4);
  Serial1.println("started..."); 
}

// Arduino loop - copy sound to out 
void loop() {
  copier.copy();
}