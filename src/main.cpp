#include "DMXUSB.h"
#include <DmxOutput.h>
#include <Arduino.h>

#define DMXUSB_BAUDRATE 115200

DmxOutput dmx;
uint8_t universe[512 + 1];


void myDMXCallback(int univNo, char buffer[512]) {

  Serial1.println("- DMX Callback called");

  digitalWrite(LED_BUILTIN, LOW);

  // Copy the buffer to the universe array, starting at index 1
  for (int index=0; index < 512; index++) {
    universe[index + 1] = buffer[index]; // universe starts at index 1, index 0 is start code
  }

  dmx.write(universe, 512);

  // while (dmx.busy())
  // {
  //   /* Do nothing while the DMX frame transmits */
  // }

  // Serial1.println("--- DMX fully transmitted");

  digitalWrite(LED_BUILTIN, HIGH);
}

DMXUSB myDMXUsb(
  Serial,
  DMXUSB_BAUDRATE,
  0,
  myDMXCallback
);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(DMXUSB_BAUDRATE);
  Serial1.begin(115200); // Serial1 is used for debugging
  dmx.begin(2);
}

void loop() {
  myDMXUsb.listen();

}