#include "setupSD.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


const int SD_CS_PIN = 13; // Pin for SD card chip select
const int SD_MOSI_PIN = 15; // Pin for SD card MOSI
const int SD_MISO_PIN = 12; // Pin for SD card MISO
const int SD_CLK_PIN = 14; // Pin for SD card clock


void setupSd() {

    SPI1.setSCK(SD_CLK_PIN);  // CLK
    SPI1.setTX(SD_MOSI_PIN);  // MOSI
    SPI1.setRX(SD_MISO_PIN);  // MISO


    if (!SD.begin(SD_CS_PIN, SPI1)) {  // CS
        Serial1.println("SD init failed!");
        while (1) {
        Serial1.println("SD init failed!");
            delay(1000);  // wait for a second
        }
    }

}