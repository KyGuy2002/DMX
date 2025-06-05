#include "setup/setupSD.h"
#include "setup/setupEthernet.h"
#include "setup/setupArtnet.h"

#include <Arduino.h>
#include <Ethernet.h>
#include <Artnet.h>
#include <DmxOutput.h>
#include <pico/multicore.h>
#include <SD.h>


void loop2();


const int DMX_PIN = 2; // Pin for DMX PIO data


#define DMXS_MAGIC "PROJECTDMX-DMXSHOW"
#define DMXS_VERSION 1


EthernetUDP udp;
ArtnetReceiver artnet;
DmxOutput dmx;

uint8_t currentDMXState[512];



void artnetCallback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {

    // Update the current DMX state
    memcpy(currentDMXState, data, size);

}


void writeDmx() {

    uint8_t output[512 + 1];

    // Copy and shift array once (dmx protocol requires a start code)
    memcpy(output + 1, currentDMXState, 512);
    
    dmx.write(output, 512);
    while (dmx.busy()) {}

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


    Serial1.println("Initialization complete. Starting DMX output...");


    // Start second core loop
    multicore_launch_core1([]() {
        while (true) {
            loop2();
        }
    });
}


void loop() {
  artnet.parse();
}

// NOTE: This function cannot be named loop1 because it conflicts with a USB function!
void loop2() {
    writeDmx();
    delay(1);
}