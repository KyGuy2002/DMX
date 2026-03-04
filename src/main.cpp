#include <Arduino.h>
#include "pins.h"
#include "settings.h"

#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include <Ethernet3.h>
#include <ArduinoMDNS.h>

using namespace audio_tools;


// ========== Audio Objects
I2SStream i2s;
MP3DecoderHelix mp3;
VolumeStream volume(i2s);
EncodedAudioStream decoder(&volume, &mp3);
StreamCopy copier;
File audioFile;


// ========== Ethernet Objects
EthernetUDP udp;
MDNS mdns(udp);
EthernetServer server(80);


void setup(void) {  

  // ========== Derived constants
  char MDNS_NAME[10];
  snprintf(MDNS_NAME, sizeof(MDNS_NAME), "pdmx-%02x%02x", MAC_ADDRESS[4], MAC_ADDRESS[5]);


  Serial1.begin(115200);
  Serial1.println();
  Serial1.println();
  Serial1.println("Starting ProjectDMX Controller...");
  


  // ========== SPI Init
  SPI.setSCK(SCK_PIN);
  SPI.setTX(MOSI_PIN);
  SPI.setRX(MISO_PIN);
  pinMode(ETH_CS_PIN, OUTPUT);
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(ETH_CS_PIN, HIGH);
  digitalWrite(SD_CS_PIN, HIGH);
  SPI.begin();



  // ========== Ethernet Init
  Ethernet.setCsPin(ETH_CS_PIN);
  Ethernet.setHostname(MDNS_NAME);
  if (Ethernet.begin(MAC_ADDRESS) == 0) {
    Serial1.println("- [X] Ethernet init failed!");
    while (true) delay(1000);
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



  // ========== SD Card Init
  if (!SD.begin(SD_CS_PIN, SPI)) {
    Serial1.println("- [X] SD init failed!");
    while (true) delay(1000);
  }
  Serial1.println("- [*] SD Card initialized successfully.");



  // ========== Open MP3 File
  audioFile = SD.open(MP3_PATH, FILE_READ);
  if (!audioFile) {
    Serial1.println("- [X] Failed to open MP3 file: " + String(MP3_PATH));
    while (true) delay(1000);
  }
  Serial1.println("- [*] MP3 file opened successfully.");



  // ========== I2S Audio Init
  I2SConfig config = i2s.defaultConfig(TX_MODE);
  config.sample_rate = 48000;
  config.bits_per_sample = 16;
  config.channels = 2;
  config.buffer_size = 2048;
  config.pin_bck = AUDIO_BCK_PIN;
  config.pin_ws = AUDIO_LCK_PIN;
  config.pin_data = AUDIO_DIN_PIN;
  if (!i2s.begin(config)) {
    Serial1.println("- [X] Failed to initialize I2S");
    while (true) delay(1000);
  }
  decoder.addNotifyAudioChange(i2s);
  volume.setVolume(0.5);
  decoder.begin();
  copier.begin(decoder, audioFile);
  Serial1.println("- [*] I2S Audio initialized successfully.");



  Serial1.println("Startup complete.");
  Serial1.println();
  Serial1.println();

}



void loop() {
  mdns.run();

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an HTTP request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the HTTP request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}



// if (!copier.copy()) {
//     Serial.println("Done (or copy error). Rewinding...");
//     audioFile.seek(0);
//     copier.begin(decoder, audioFile);
//     delay(200);
//   }


