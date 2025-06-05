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

#define MAX_UNIVERSES 2 // Maximum number of universes supported

const String magic = "PROJECT-DMX-DMXS"; // 16 bytes (ascii)

const int DMX_PIN = 2; // Pin for DMX PIO data


uint8_t **prev = allocDMXArray(MAX_UNIVERSES);
uint8_t **curr = allocDMXArray(MAX_UNIVERSES);

EthernetUDP udp;
ArtnetReceiver artnet;
DmxOutput dmx;

uint32_t lastSaved = 0;




void artnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    Serial1.println("Received Art-Net DMX data");

    // Update current dmx data
    for (uint16_t i = 0; i < size; ++i) {
        if (metadata.universe < MAX_UNIVERSES && i < 512) { // Ensure we don't go out of bounds
            curr[metadata.universe][i] = data[i];
        }
    }

    // Write the DMX frame to the file
    writeDMXFrame(dmxFile, prev, curr, MAX_UNIVERSES);

    // Copy current to previous
    memccpy(prev[metadata.universe], curr[metadata.universe], 512, sizeof(uint8_t) * 512);

    // Save DMX frame to file
    if (millis() - lastSaved > 1000) {  // Save every second
        dmxFile.flush();
        lastSaved = millis();
        Serial1.println("DMX data saved to file.");
    }

}





void setup() {
    Serial1.begin(115200);

    Serial1.println("Starting ProjectDMX LumaKit...");

    // Initialize connections
    setupEthernet();  // Initialize Ethernet
    setupSd();  // Initialize SD card
    setupArtnet(artnet);  // Initialize Art-Net
    artnet.subscribeArtDmxUniverse(0, artnetCallback);
    dmx.begin(DMX_PIN);

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



    dmxFile.flush();
    Serial1.println("Started Complete!");



}


void loop() {
    artnet.parse();
}