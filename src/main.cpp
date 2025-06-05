#include "setup/setupSD.h"
#include "setup/setupEthernet.h"
#include "setup/setupArtnet.h"
#include "utils/utils.h"

#include <Arduino.h>
#include <Ethernet.h>
#include <Artnet.h>
#include <DmxOutput.h>
#include <pico/multicore.h>
#include <SD.h>
#include <iostream>
#include <chrono>


File dmxFile;

String magic = "PROJECT-DMX-DMXS"; // 16 bytes (ascii)




void setup() {
    Serial1.begin(115200);

    delay(1000);  // Wait for Serial to be ready

    Serial1.println("Starting ProjectDMX LumaKit...");


    // Initialize connections
    setupSd();  // Initialize SD card

    String name = "test2.dmxs";
    if (SD.exists(name)) SD.remove(name);  // Deletes the old file

    dmxFile = SD.open(name, FILE_WRITE);


    if (!dmxFile) {
        while (1) {
            Serial1.println("Failed to create DMX file");
            delay(1000);  // Wait
        }
    }



    dmxFile.print(magic); // 16 bytes

    // Version: 1
    writeUInt8(dmxFile, 1);

    // Universes: 1
    writeUInt8(dmxFile, 1);

    // Duration seconds: 0 (Placeholder)
    writeUInt16(dmxFile, 6*60*60); // 6hrs in seconds

    // Reserved: 12 bytes of 0
    for (int i = 0; i < 12; ++i) {
        writeUInt8(dmxFile, 0);
    }




    // uint8_t **prev = allocDMXArray(2);
    // uint8_t **curr = allocDMXArray(2);

    // curr[0][12] = 255;  // Set channel 12 of universe 0 to 255
    // curr[1][8] = 255;  // Set channel 8 of universe 1 to 255

    // writeDMXFrame(dmxFile, prev, curr, 2);


    dmxFile.close();
    Serial1.println("Done.");
    Serial1.println();

    File f = SD.open(name, FILE_READ);

    while (f.available()) {
        uint8_t b = f.read();
        if (b < 0x10) Serial1.print("0");  // leading zero for clean hex
        Serial1.print(b, HEX);
        Serial1.print(" ");
    }

    Serial1.println();
    Serial1.println("DMX file created successfully.");



}


void loop() {

}