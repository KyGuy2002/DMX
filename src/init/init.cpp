#include <Arduino.h>
#include <SPI.h>
#include "pins.h"


void initPeripherals() {
  Serial1.println("Initializing peripherals...");

  
  initSPI();

  


}



void initSPI() {
    SPI.setSCK(SCK_PIN);
    SPI.setTX(MOSI_PIN);
    SPI.setRX(MISO_PIN);
    pinMode(ETH_CS_PIN, OUTPUT); // Ethernet
    pinMode(SD_CS_PIN, OUTPUT); // SD Card
    digitalWrite(ETH_CS_PIN, HIGH); // Ethernet
    digitalWrite(SD_CS_PIN, HIGH); // SD Card
    SPI.begin();
    
    Serial1.println("- [*] SPI initialized successfully.");
}