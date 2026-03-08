#include "task_oled.h"
#include "../peripherals/peripheral_init.h"
#include "../rtos_config.h"
#include <Ethernet3.h>

TaskHandle_t oledTaskHandle = NULL;


void oledTask(void *pvParameters) {
  Serial1.println("[OLED Task] Started");
  
  while (true) {
    u8g2.clearBuffer();
    
    
    // Header
    u8g2.drawBox(0, 0, 128, 10);
    u8g2.setDrawColor(0);
    u8g2.setFont(u8g2_font_helvR08_tr);

    int w = u8g2.getStrWidth("DMX Addr");
    int x = (128 - w) / 2;
    u8g2.setCursor(x, 9);
    u8g2.print("DMX Addr");
    u8g2.setDrawColor(1);

    // Address
    u8g2.setFont(u8g2_font_logisoso38_tr);
    int w2 = u8g2.getStrWidth("129");
    int x2 = (128 - w2) / 2;
    u8g2.setCursor(x2, 55);
    u8g2.print("129");


    u8g2.setFont(u8g2_font_helvR08_tr);
    u8g2.setFontPosBottom();
    u8g2.setCursor(64, 100);

    u8g2.setFontPosBaseline();
    u8g2.sendBuffer();

    vTaskDelay(pdMS_TO_TICKS(OLED_TASK_DELAY_MS));
  }
  
}

void createOLEDTask() {
  xTaskCreate(
    oledTask,                 // Task function
    "OLED",                   // Task name
    OLED_TASK_STACK_SIZE,     // Stack size (bytes)
    NULL,                     // Parameters
    OLED_TASK_PRIORITY,       // Priority
    &oledTaskHandle           // Task handle
  );
}
