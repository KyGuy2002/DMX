#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

File myFile;

const int SD_CS_PIN = 13; // Pin for SD card chip select
const int SD_MOSI_PIN = 15; // Pin for SD card MOSI
const int SD_MISO_PIN = 12; // Pin for SD card MISO
const int SD_CLK_PIN = 14; // Pin for SD card clock

void setup(void) {  

  // SPI1.setCS(13);
  // SPI1.begin();
  
  Serial1.begin(115200);
  delay(1500);

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
  Serial1.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial1.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial1.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial1.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial1.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial1.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial1.println("error opening test.txt");
  }

}

void loop() {
}