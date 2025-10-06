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


// Mode: RECORD or Play
const bool RECORD = false;


File dmxFile;

#define MAX_UNIVERSES 2 // Maximum number of universes supported

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


void pushDmxFrame() {
    Serial1.println("Pushing DMX frame...");


    readDMXFrameDelta(dmxFile, prev, curr, MAX_UNIVERSES); // Does nothing at end of file
    // file.seek(0);

    // Push the DMX frame to the output
    for (uint16_t u = 0; u < MAX_UNIVERSES; u++) {

        uint8_t output[512 + 1];

        // Copy and shift array once (dmx protocol requires a start code)
        memcpy(output + 1, curr[u], 512);

        dmx.write(output, 512);
    }

    while (dmx.busy()) {
        // Wait for the DMX frame to be sent
    }

    // delay(1000/44);  // Wait for the next frame (44Hz)


}





void setup() {
    Serial1.begin(115200);

    Serial1.println("Starting ProjectDMX LumaKit...");

    // Initialize connections
    if (RECORD) setupEthernet();  // Initialize Ethernet
    setupSd();  // Initialize SD card
    if (RECORD) setupArtnet(artnet);  // Initialize Art-Net
    if (RECORD) artnet.subscribeArtDmxUniverse(0, artnetCallback);
    if (!RECORD) dmx.begin(DMX_PIN);

    String name = "test2.dmxs";
    if (RECORD && SD.exists(name)) SD.remove(name);  // Deletes the old file

    dmxFile = SD.open(name, (RECORD ? FILE_WRITE : FILE_READ));


    if (!dmxFile) {
        while (1) {
            Serial1.println("Failed to read/write DMX file!");
            delay(1000);  // Wait
        }
    }


    if (RECORD) writeFileHeader(dmxFile, name, MAX_UNIVERSES);  // Write file header
    if (RECORD) dmxFile.flush();


    Serial1.println("Startup Complete!");

}


void loop() {
    if (RECORD) artnet.parse();
    if (!RECORD) pushDmxFrame();
}