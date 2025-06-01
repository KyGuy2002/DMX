#include "DMXUSB.h"

#define DMXUSB_BAUDRATE 115200


void myDMXCallback(int universe, char buffer[512]) {
  for (int index=0; index < 512; index++) { // for each channel, universe starts at 0
    int channel = index + 1; // channel starts at 0, so index 0 is DMX channel 1 and index 511 is DMX channel 512
    int value = buffer[index]; // DMX value 0 to 255
    if (universe == 0 && channel == 1) analogWrite(LED_BUILTIN, value); // LED on channel 1 on universe 0
  }
}

DMXUSB myDMXUsb(
  Serial,
  DMXUSB_BAUDRATE,
  0,
  myDMXCallback
);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(DMXUSB_BAUDRATE);
}

void loop() {
  myDMXUsb.listen();
}