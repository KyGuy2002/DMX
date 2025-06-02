#include "DMXUSB.h"
#include <DmxOutput.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#define DMXUSB_BAUDRATE 115200

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetServer server(80); // HTTP port

DmxOutput dmx;
uint8_t universe[512 + 1];


void myDMXCallback(int univNo, char buffer[512]) {

  Serial1.println("- DMX Callback called");

  digitalWrite(LED_BUILTIN, LOW);

  // Copy the buffer to the universe array, starting at index 1
  for (int index=0; index < 512; index++) {
    universe[index + 1] = buffer[index]; // universe starts at index 1, index 0 is start code
  }

  dmx.write(universe, 512);

  // while (dmx.busy())
  // {
  //   /* Do nothing while the DMX frame transmits */
  // }

  // Serial1.println("--- DMX fully transmitted");

  digitalWrite(LED_BUILTIN, HIGH);
}

DMXUSB myDMXUsb(
  Serial,
  DMXUSB_BAUDRATE,
  0,
  myDMXCallback
);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(DMXUSB_BAUDRATE);
  Serial1.begin(115200); // Serial1 is used for debugging

  Serial1.println("Starting up...");

  // CS pin = GP17
  Ethernet.init(17);

  Serial1.println("Starting Ethernet...");

  // IPAddress ip(192, 168, 1, 177);
  // Ethernet.begin(mac, ip);
  if (Ethernet.begin(mac) == 0) {
    Serial1.println("!! Failed to configure Ethernet using DHCP");
    // Optional fallback to static IP
    // IPAddress ip(192, 168, 1, 177);
    // Ethernet.begin(mac, ip);
    while (true) {
      Serial1.println("!! STOPPED: Failed to configure Ethernet using DHCP");
      delay(1000); // Stop here or show error
    }
  }

  Serial1.println("Success");

  delay(1000);
  Serial1.print("My IP: ");
  Serial1.println(Ethernet.localIP());

  server.begin();

  // digitalWrite(LED_BUILTIN, HIGH);

  dmx.begin(2);

  Serial1.println("Setup complete!");
}


void loop() {
  EthernetClient client = server.available();
  if (client) {
    String request = "";
    bool currentLineIsBlank = false;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n' && currentLineIsBlank) {
          // Check for /led in GET request
          if (request.indexOf("GET /led") >= 0) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
          }

          // Send HTML response
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html><html><body>");
          client.println("<h1>Pico W5500 Web Control</h1>");
          client.println("<button onclick=\"location.href='/led'\">Flash LED</button>");
          client.println("</body></html>");
          break;
        }
        if (c == '\n') currentLineIsBlank = true;
        else if (c != '\r') currentLineIsBlank = false;
      }
    }
    delay(1);
    client.stop();
  }
}

// void loop() {
//   myDMXUsb.listen();

// }