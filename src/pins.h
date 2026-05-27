#pragma once


// Debug Probe
// GP0
// GP1


// SD Card (SPI0)
static const int SD_SCK_PIN = 34;
static const int SD_MOSI_PIN = 35;
static const int SD_MISO_PIN = 36;
static const int SD_CS_PIN = 37;

// Buttons
static const int BUTTON_MENU_PIN = 33;
static const int BUTTON_DOWN_PIN = 31;
static const int BUTTON_UP_PIN = 32;
static const int BUTTON_ENTER_PIN = 30;

// Ethernet (SPI1) [Seperate Bus]
static const int ETH_SCK_PIN = 10;
static const int ETH_MOSI_PIN = 11;
static const int ETH_MISO_PIN = 12;
static const int ETH_CS_PIN = 13;



// =========== OLED (I2C1)
static const int I2C_SDA_PIN = 38;
static const int I2C_SCL_PIN = 27; // 39




// =========== DMX (UART1)
static const int DMX_TX_PIN = 24;
static const int DMX_RX_PIN = 25;
static const int DMX_MODE_PIN = 23;



// =========== I2S Audio
// NOTE: LCK/WS MUST BE BCK+1 (one more than BCK) [lck > bck]
static const int AUDIO_DIN_PIN = 20;
static const int AUDIO_BCK_PIN = 21;
static const int AUDIO_LCK_PIN = 22;



// Unused/Extra Pins
static const int UNUSED_PIN_1 = 27;
static const int UNUSED_PIN_2 = 26;
static const int UNUSED_PIN_3 = 29;
static const int UNUSED_PIN_4 = 28;



// =========== Modules

// Module D
static const int MODULE_D_PIN_4 = 16;
static const int MODULE_D_PIN_3 = 17;
static const int MODULE_D_PIN_2 = 18;
static const int MODULE_D_PIN_1 = 19;

// Module C
static const int MODULE_C_PIN_4 = 7;
static const int MODULE_C_PIN_3 = 9; // 1st Neo Output
static const int MODULE_C_PIN_2 = 14;
static const int MODULE_C_PIN_1 = 15;

// Module B [Analog]
static const int MODULE_B_PIN_3 = 5;
static const int MODULE_B_PIN_2 = 6;
static const int MODULE_B_PIN_1 = 8;
static const int MODULE_B_PIN_A4 = 41; // Analog

// Module A [Analog]
static const int MODULE_A_PIN_3 = 2;
static const int MODULE_A_PIN_2 = 3;
static const int MODULE_A_PIN_1 = 4;
static const int MODULE_A_PIN_A4 = 40; // Analog



// ========== Inputs
static const int INPUT_6_PIN = 47;
static const int INPUT_5_PIN = 45;
static const int INPUT_4_PIN = 46;
static const int INPUT_3_PIN = 44;
static const int INPUT_2_PIN = 43;
static const int INPUT_1_PIN = 42;