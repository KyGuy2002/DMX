#include "peripheral_init.h"
#include "../rtos_config.h"

// Global peripheral objects
I2SStream i2s;
MP3DecoderHelix mp3;
VolumeStream volume(i2s);
EncodedAudioStream decoder(&volume, &mp3);
EthernetUDP udp;
MDNS mdns(udp);
EthernetServer server(80);
char MDNS_NAME[10];

bool initSPI() {
  SPI.setSCK(SCK_PIN);
  SPI.setTX(MOSI_PIN);
  SPI.setRX(MISO_PIN);
  pinMode(ETH_CS_PIN, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(ETH_CS_PIN, HIGH);
  digitalWrite(SD_CS_PIN, HIGH);
  SPI.begin();
  
  Serial1.println("- [*] SPI initialized successfully.");
  return true;
}

bool initEthernet() {
  // Generate MDNS name
  snprintf(MDNS_NAME, sizeof(MDNS_NAME), "pdmx-%02x%02x", MAC_ADDRESS[4], MAC_ADDRESS[5]);
  
  Ethernet.setCsPin(ETH_CS_PIN);
  Ethernet.setHostname(MDNS_NAME);
  
  if (Ethernet.begin(MAC_ADDRESS) == 0) {
    Serial1.println("- [X] Ethernet init failed!");
    return false;
  }
  
  server.begin();
  mdns.begin(Ethernet.localIP(), MDNS_NAME);
  mdns.addServiceRecord(MDNS_NAME, 80, MDNSServiceTCP);
  
  Serial1.println("- [*] Ethernet initialized successfully.");
  Serial1.print("      IP Address: ");
  Serial1.println(Ethernet.localIP());
  Serial1.print("      Hostname: ");
  Serial1.print(MDNS_NAME);
  Serial1.println(" (.local) (.lan)");
  
  return true;
}

bool initSDCard() {
  if (!SD.begin(SD_CS_PIN, SPI)) {
    Serial1.println("- [X] SD init failed!");
    return false;
  }
  
  Serial1.println("- [*] SD Card initialized successfully.");
  return true;
}

bool initI2SAudio() {
  I2SConfig config = i2s.defaultConfig(TX_MODE);
  config.sample_rate = 48000;
  config.bits_per_sample = 16;
  config.channels = 2;
  config.buffer_size = 1280;  // Larger buffer for complex audio sections (~27ms at 48kHz)
  config.buffer_count = 8;
  config.pin_bck = AUDIO_BCK_PIN;
  config.pin_ws = AUDIO_LCK_PIN;
  config.pin_data = AUDIO_DIN_PIN;
  
  if (!i2s.begin(config)) {
    Serial1.println("- [X] Failed to initialize I2S");
    return false;
  }
  
  decoder.addNotifyAudioChange(i2s);
  volume.setVolume(DEFAULT_VOLUME);
  decoder.begin();
  
  Serial1.println("- [*] I2S Audio initialized successfully.");
  return true;
}
