#pragma once


// SD Card (SPI(0))
static const int SD_MOSI_PIN = 3;
static const int SD_MISO_PIN = 4;
static const int SD_SCK_PIN = 2;
static const int SD_CS_PIN = 5;

// Ethernet (SPI1) [Seperate Bus]
static const int ETH_MISO_PIN = 12;
static const int ETH_SCK_PIN = 10;
static const int ETH_MOSI_PIN = 11;
static const int ETH_CS_PIN = 13;



// =========== I2C (OLED)
static const int I2C_SDA_PIN = 6;
static const int I2C_SCL_PIN = 7;

// Buttons
static const int BUTTON_MENU_PIN = 8;
static const int BUTTON_UP_PIN = 19;
static const int BUTTON_DOWN_PIN = 15;
static const int BUTTON_ENTER_PIN = 14;



// =========== DMX
static const int DMX_TX_PIN = 16;
static const int DMX_RX_PIN = 17;
static const int DMX_MODE_PIN = 18;



// =========== I2S Audio
// NOTE: LCK/WS MUST BE BCK+1 (one more than BCK) [lck > bck]
static const int AUDIO_LCK_PIN = 22;
static const int AUDIO_DIN_PIN = 20;
static const int AUDIO_BCK_PIN = 21;



// =========== Modules
static const int MODULE_A_PIN_1 = 9;
static const int MODULE_A_PIN_2 = 26; // Bottom 1/4 Neo Dev Breadboard pin
static const int MODULE_A_PIN_3 = 27;
static const int MODULE_A_PIN_4 = 28; // Analog???



// ========== Inputs
static const int INPUT_1_PIN = 40; // CHANGED FROM 29 TO 40 RP2040