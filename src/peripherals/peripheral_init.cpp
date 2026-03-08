#include "peripheral_init.h"
#include "../rtos_config.h"


// Mutex
SemaphoreHandle_t xSPIMutex = xSemaphoreCreateMutex();


// Global peripheral objects
I2SStream i2s;
MP3DecoderHelix mp3;
VolumeStream volume(i2s);
EncodedAudioStream decoder(&volume, &mp3);
EthernetUDP udp;
MDNS mdns(udp);
EthernetServer server(80);
char MDNS_NAME[10];
// [ rotation, reset pin ]
U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
// MUIU8G2 mui;

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
  // Generate MDNS name (used for url/hostname [pdmx-xxxx.local])
  snprintf(MDNS_NAME, sizeof(MDNS_NAME), "pdmx-%02x%02x", MAC_ADDRESS[4], MAC_ADDRESS[5]);
  
  Ethernet.setCsPin(ETH_CS_PIN);
  Ethernet.setHostname(MDNS_NAME);
  
  if (Ethernet.begin(MAC_ADDRESS) == 0) {
    Serial1.println("- [X] Ethernet init failed!");
    return false;
  }
  
  server.begin();
  if (!mdns.begin(Ethernet.localIP(), MDNS_NAME)) {
    Serial1.println("- [X] mDNS responder init failed!");
    return false;
  }

  char SERVICE_NAME[30]; // Used for pretty mDNS service name like "ProjectDMX xxxx" (._http part is not shown on mDNS clients but required)
  snprintf(SERVICE_NAME, sizeof(SERVICE_NAME), "ProjectDMX %02x%02x._http", MAC_ADDRESS[4], MAC_ADDRESS[5]);
  Serial1.println(SERVICE_NAME);
  if (!mdns.addServiceRecord(SERVICE_NAME, 80, MDNSServiceTCP)) {
    Serial1.println("- [X] mDNS HTTP service registration failed!");
    return false;
  }
  
  Serial1.println("- [*] Ethernet initialized successfully.");
  Serial1.print("      IP Address: ");
  Serial1.println(Ethernet.localIP());
  Serial1.print("      Hostname: ");
  Serial1.print(MDNS_NAME);
  Serial1.println(" (.local)");
  
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
  config.buffer_size = AUDIO_TASK_STACK_SIZE;
  // config.buffer_count = 8;
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

bool initOLED() {

  Wire1.setSCL(7);
  Wire1.setSDA(6);
  Wire1.begin();
  Wire1.beginTransmission(0x3C);
  u8g2.setI2CAddress(0x3C << 1);
  // [ select, next, prev, up, down, home ]
  u8g2.begin(BUTTON_ENTER_PIN, BUTTON_UP_PIN, BUTTON_DOWN_PIN, U8X8_PIN_NONE, U8X8_PIN_NONE, BUTTON_MENU_PIN);
  // mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));

  Serial1.println("- [*] OLED initialized successfully.");
  return true;

}