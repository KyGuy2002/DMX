#include "setupEthernet.h"

#include <Arduino.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


void setupEthernet() {

    Ethernet.init(17);
    if (Ethernet.begin(mac) == 0) {
        Serial1.println("!! Failed to configure Ethernet using DHCP");
        while (true) {
            Serial1.println("!! STOPPED: Failed to configure Ethernet using DHCP");
            delay(1000);
        }
    }

}