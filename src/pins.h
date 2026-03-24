#pragma once


// Debug Probe
// GP0
// GP1


// SD Card (SPI(0))
static const int SD_SCK_PIN = 2;
static const int SD_MOSI_PIN = 3;
static const int SD_MISO_PIN = 4;
static const int SD_CS_PIN = 5;

// Buttons
static const int BUTTON_MENU_PIN = 6;
static const int BUTTON_DOWN_PIN = 7;
static const int BUTTON_UP_PIN = 8;
static const int BUTTON_ENTER_PIN = 9;

// Ethernet (SPI1) [Seperate Bus]
static const int ETH_SCK_PIN = 10;
static const int ETH_MOSI_PIN = 11;
static const int ETH_MISO_PIN = 12;
static const int ETH_CS_PIN = 13;



// =========== I2C (OLED)
static const int I2C_SDA_PIN = 14;
static const int I2C_SCL_PIN = 15;




// =========== DMX
static const int DMX_TX_PIN = 16;
static const int DMX_RX_PIN = 17;
static const int DMX_MODE_PIN = 18;



// =========== I2S Audio
// NOTE: LCK/WS MUST BE BCK+1 (one more than BCK) [lck > bck]
static const int AUDIO_DIN_PIN = 19;
static const int AUDIO_BCK_PIN = 20;
static const int AUDIO_LCK_PIN = 21;



// Unused/Extra Pins
static const int UNUSED_PIN_1 = 22;
static const int UNUSED_PIN_2 = 23;
static const int UNUSED_PIN_3 = 24;
static const int UNUSED_PIN_4 = 25;



// =========== Modules

// Module D [AMP]
static const int MODULE_D_PIN_4 = 26;
static const int MODULE_D_PIN_3 = 27;
static const int MODULE_D_PIN_2 = 28;
static const int MODULE_D_PIN_1 = 29;

// Module C
static const int MODULE_C_PIN_4 = 30;
static const int MODULE_C_PIN_3 = 31;
static const int MODULE_C_PIN_2 = 32;
static const int MODULE_C_PIN_1 = 33;

// Module B [Analog]
static const int MODULE_B_PIN_3 = 34;
static const int MODULE_B_PIN_2 = 35;
static const int MODULE_B_PIN_1 = 36;
static const int MODULE_B_PIN_A4 = 40; // Analog

// Module A [Analog]
static const int MODULE_A_PIN_3 = 37;
static const int MODULE_A_PIN_2 = 38;
static const int MODULE_A_PIN_1 = 39;
static const int MODULE_A_PIN_A4 = 41; // Analog



// ========== Inputs
static const int INPUT_6_PIN = 42;
static const int INPUT_5_PIN = 43;
static const int INPUT_4_PIN = 44;
static const int INPUT_3_PIN = 45;
static const int INPUT_2_PIN = 46;
static const int INPUT_1_PIN = 47;