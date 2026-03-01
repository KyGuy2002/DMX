#include <Arduino.h>
#include "AudioTools.h"
#include "untitled.h"

AudioInfo info(16000, 1, 16);
I2SStream out;
MemoryStream music(untitled_raw, untitled_raw_len);
StreamCopy copier(out, music); // copies sound into i2s

// Arduino Setup
void setup(void) {  
  // Open Serial 
  Serial1.begin(115200);
  AudioToolsLogger.begin(Serial1, AudioToolsLogLevel::Info);

  // start i2s
  Serial1.println("starting I2S...");
  I2SConfig config = out.defaultConfig(TX_MODE);
  config.copyFrom(info);
  config.buffer_size = 2048;
  out.begin(config);

  delay(50);
}

// Arduino loop - copy sound to out 
void loop() {
  if (!copier.copy()){
      out.end();
      stop();
    }
}