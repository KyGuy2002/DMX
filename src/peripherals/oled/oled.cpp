#include "oled.h"
#include "oled_loading_task.h"


// [ rotation, reset pin ]
U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);



void createOLEDInitTask() {
  xTaskCreate(
    oledInitTask,             // Task function
    "OLED Init",              // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void oledInitTask(void *pvParameters) {

  Wire1.setSCL(I2C_SCL_PIN);
  Wire1.setSDA(I2C_SDA_PIN);
  Wire1.begin();
  Wire1.beginTransmission(0x3C);
  uint8_t err = Wire1.endTransmission();   // 0 = device ACK
  if (err != 0) {
    Serial1.println("- [X] Failed to initialize OLED: I2C device did not ACK");
    initSyncMarkDone(INIT_OLED_DONE, false, INIT_OLED_OK);
    vTaskDelete(NULL);
    return;
  }

  u8g2.setI2CAddress(0x3C << 1);
  // [ select, next, prev, up, down, home ]
  u8g2.begin(BUTTON_ENTER_PIN, BUTTON_UP_PIN, BUTTON_DOWN_PIN, U8X8_PIN_NONE, U8X8_PIN_NONE, BUTTON_MENU_PIN);


  Serial1.println("- [*] OLED initialized successfully.");
  initSyncMarkDone(INIT_OLED_DONE, true, INIT_OLED_OK);

  // Start loading spinner screen (ends itself)
  createOLEDStartupSplashTask();

  vTaskDelete(NULL);

}