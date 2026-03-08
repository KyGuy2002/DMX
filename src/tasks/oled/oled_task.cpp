#include "oled_task.h"


void createOLEDTask() {
  xTaskCreate(
    oledTask,             // Task function
    "OLED Task",    // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void oledTask(void *pvParameters) {
    while (1) {


        u8g2.clearBuffer();
        oledDrawCenterText(u8g2, "ProjectDMX Home", 128, 32);
        u8g2.sendBuffer();


        vTaskDelay(pdMS_TO_TICKS(50));
    }
}