#include <Arduino.h>
#include "AudioTools.h"
// #include "audio5.h"

AudioInfo info(48000, 2, 16);
I2SStream out;
// MemoryStream sound(audio5_raw, audio5_raw_len);
SineWaveGenerator<int16_t> sine;
GeneratedSoundStream<int16_t> sound(sine);
StreamCopy copier(out, sound); // copies sound into i2s

// Arduino Setup
void setup(void) {  
  // Open Serial 

  sine.begin(info, 440);

  // start i2s
  I2SConfig config = out.defaultConfig(TX_MODE);
  config.copyFrom(info);
  config.buffer_size = 2048;
  out.begin(config);

  delay(50);
}

// Arduino loop - copy sound to out 
void loop() {
  copier.copy();
}