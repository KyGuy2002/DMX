#include "oled_error_task.h"


char title[30];
char desc[30];


void createOLEDErrorTask(char *title, char *desc) {
    strncpy(title, title, sizeof(title) - 1);
    title[sizeof(title) - 1] = '\0';
    strncpy(desc, desc, sizeof(desc) - 1);
    desc[sizeof(desc) - 1] = '\0';
    xTaskCreate(
      oledErrorTask,             // Task function
      "OLED ErrorTask",    // Task name
      2048,                     // Stack size (bytes)
      NULL,                     // Parameters
      3,                        // Priority
      NULL
    );
}




void oledErrorTask(void *pvParameters) {
  while (1) {
    
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
    u8g2.drawBox(0, 0, 128, 64);
    u8g2.setDrawColor(0);
    oledDrawCenterText(u8g2, title, 128, 24);
    oledDrawCenterText(u8g2, desc, 128, 40);
    u8g2.sendBuffer();

  }
}