#include "setup/setupSD.h"
#include "setup/setupEthernet.h"
#include "setup/setupArtnet.h"

#include <Arduino.h>
#include <Ethernet.h>
#include <Artnet.h>
#include <DmxOutput.h>
#include <pico/multicore.h>
#include <SD.h>
#include <iostream>
#include <chrono>


void loop2();


const int DMX_PIN = 2; // Pin for DMX PIO data


#define DMXS_MAGIC "PROJECTDMX-DMXSHOW"
#define DMXS_VERSION 1


EthernetUDP udp;
ArtnetReceiver artnet;
DmxOutput dmx;

uint8_t lastDMX[1][512] = {{0}};  // previous state
File dmxFile;
uint32_t startTime;
uint32_t lastSaved = 0;


struct DMXChange {
  uint16_t channel;
  uint8_t value;
};


void artnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    Serial1.println("Received DMX data from Art-Net");
    DMXChange changes[512];
    uint16_t count = 0;

    for (uint16_t i = 0; i < 512; ++i) {
        if (data[i] != lastDMX[0][i]) {
        changes[count].channel = i;
        changes[count].value = data[i];
        lastDMX[0][i] = data[i];
        ++count;
        }
    }

    if (count == 0) {
        Serial1.println("No changes detected, skipping file write.");
        return;  // No changes
    }

    uint32_t timestamp = millis() - startTime;
    dmxFile.write((uint8_t*)&timestamp, 4);
    dmxFile.write((uint8_t*)0, 2);
    dmxFile.write((uint8_t*)&count, 2);

    for (uint16_t i = 0; i < count; ++i) {
        dmxFile.write((uint8_t*)&changes[i].channel, 2);
        dmxFile.write(changes[i].value);
    }

    Serial1.print("Written");

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

    // Open new save file
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    // String name = now_ms + "-save.dmxs";
    String name = "test.dmxs";

    if (SD.exists(name)) SD.remove(name);  // Deletes the old file

    Serial1.print("Saving to file: ");
    Serial1.println(name);

    dmxFile = SD.open(name, FILE_WRITE);

    if (!dmxFile) {
        while (1) {
            Serial1.println("Failed to create DMX file");
            delay(1000);  // Wait
        }
    }

    dmxFile.print("hello");

    const char magic[19] = "PROJECTDMX-DMXSHOW";
    dmxFile.write((const uint8_t*)magic, 18); // Omit null terminator
    dmxFile.write((uint8_t)1);                    // version
    dmxFile.write((uint16_t)1);   // 2-byte universe count
    for (int i = 0; i < 11; ++i) dmxFile.write((uint8_t)0); // reserved

    startTime = millis();


    dmxFile.flush();


    Serial1.println("Initialization complete. Starting DMX output...");


    // Start second core loop
    // multicore_launch_core1([]() {
    //     while (true) {
    //         loop2();
    //     }
    // });
}


void loop() {
  artnet.parse();
}

// NOTE: This function cannot be named loop1 because it conflicts with a USB function!
// void loop2() {
//     Serial1.println("==== Saved ====");
    
//     // Write to file
//     dmxFile.flush();
//     delay(1000);
// }