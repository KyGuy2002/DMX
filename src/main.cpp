#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


String data;


void setup() {
  Serial1.begin(115200);
  Serial1.println("Starting up...");
  
  SPI1.setSCK(14);  // CLK
  SPI1.setTX(15);  // MOSI
  SPI1.setRX(12);  // MISO

  Serial1.println("first part complete!");

  if (!SD.begin(13, SPI1)) { // CS
    Serial1.println("SD init failed!");
    while (1) {
      Serial1.println("SD init failed!");
      delay(1000);  // wait for a second
    }
  }
  Serial1.println("SD init successful.");

  File file = SD.open("test.txt");

  if (!file) {
    while (1) {
      Serial1.println("Failed to open file!");
      delay(1000);  // wait for a second
    }
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');  // read line from file
    line.trim();  // remove \r or trailing whitespace


  }

  file.close();
  
}

void loop() {
  

  Serial1.print("Data from SD: ");
  Serial1.println(data);  // print the data read from the file

  delay(1000);  // wait for a second before the next read


}
