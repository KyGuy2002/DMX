#include <Ethernet.h>
#include <Artnet.h>
#include <DmxOutput.h>
#include <pico/multicore.h>

// Network settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetUDP Udp;
Artnet artnet;

DmxOutput dmx;
uint8_t universe[512 + 1];

// Callback for incoming DMX data
void onDmxFrame(uint16_t uniNo, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP) {

  // Shift DMX data by one position in the universe array
  universe[0] = 0; // DMX start code
  memcpy(universe + 1, data, length);

}

void core1_entry() {
  while (true) {
    
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

  artnet.begin();
  artnet.setArtDmxCallback(onDmxFrame);

  dmx.begin(2);

  multicore_launch_core1(core1_entry);
}

void loop() {
  artnet.read();
}
