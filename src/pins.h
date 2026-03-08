#pragma once

// =========== SPI (SD Card, Ethernet)
static const int MOSI_PIN = 3;
static const int MISO_PIN = 4;
static const int SCK_PIN = 2;

// SD Card
static const int SD_CS_PIN = 5;

// Ethernet
static const int ETH_CS_PIN = 9;



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