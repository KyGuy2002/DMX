#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "pins.h"


void initOLED() {

    Wire1.setSCL(I2C_SCL_PIN);
    Wire1.setSDA(I2C_SDA_PIN);
    Wire1.begin();
    Wire1.beginTransmission(0x3C);
    u8g2.setI2CAddress(0x3C << 1);
    // [ select, next, prev, up, down, home ]
    u8g2.begin(BUTTON_ENTER_PIN, BUTTON_UP_PIN, BUTTON_DOWN_PIN, U8X8_PIN_NONE, U8X8_PIN_NONE, BUTTON_MENU_PIN);

    Serial1.println("- [*] OLED initialized successfully.");
    return true;

}