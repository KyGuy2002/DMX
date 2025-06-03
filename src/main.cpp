#include <Ethernet.h>
#include <Artnet.h>
#include <DmxOutput.h>
#include <pico/multicore.h>

// Network settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

ArtnetReceiver artnet;
uint16_t universe1 = 0;

DmxOutput dmx;
uint8_t universe[512 + 1];

void callback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {

  // Serial1.print("Received DMX data");
  // for (uint16_t i = 0; i < 10; i++) {
  //   Serial1.print(" ");
  //   Serial1.print(data[i]);
  // }
  // Serial1.println();
  
  // Shift DMX data by one position in the universe array
  universe[0] = 0; // DMX start code
  memcpy(universe + 1, data, size);

}

// Callback for incoming DMX data
// void onDmxFrame(uint16_t uniNo, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP) {

//   // Shift DMX data by one position in the universe array
//   universe[0] = 0; // DMX start code
//   memcpy(universe + 1, data, length);

// }

void core1_entry() {
  while (true) {

    Serial1.println(universe[1]);
    
    dmx.write(universe, 512);

    while (dmx.busy()) {}

    delay(1);

  }
}

void setup() {
  Serial1.begin(115200);
  Serial1.println("Starting Art-Net Receiver...");
  Ethernet.init(17);
  
  if (Ethernet.begin(mac) == 0) {
    Serial1.println("!! Failed to configure Ethernet using DHCP");
    while (true) {
      Serial1.println("!! STOPPED: Failed to configure Ethernet using DHCP");
      delay(1000);
    }
  }

  artnet.setArtPollReplyConfigLongName("Project DMX Helper");
  artnet.setArtPollReplyConfigShortName("Project DMX Helper");

  artnet.begin();

  artnet.subscribeArtDmxUniverse(universe1, callback);

  dmx.begin(2);

  multicore_launch_core1(core1_entry);
}

void loop() {
  artnet.parse();
}
