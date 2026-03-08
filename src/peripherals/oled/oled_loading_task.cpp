#include "oled_loading_task.h"
#include "../../tasks/oled/oled_task.h"


void createOLEDStartupSplashTask() {
  xTaskCreate(
    oledStartupSplashTask,             // Task function
    "OLED Startup Splash",    // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    NULL                      // Task handle
  );
}


void oledStartupSplashTask(void *pvParameters) {
    OledMsg rx{};
    while (1) {
        tickStartupSplashScreen();

        // Every tick check if any errors and if everything is done and successful

        if (xQueueReceive(g_oledQueue, &rx, 0) == pdTRUE) {
            Serial1.printf("OledMsg: title=%s desc=%s\n", rx.title, rx.desc);

            if (strcmp(rx.title, "Startup Failed") == 0) {
                // Show error message on OLED
                u8g2.clearBuffer();
                u8g2.setDrawColor(1);
                u8g2.drawBox(0, 0, 128, 64);
                u8g2.setDrawColor(0);
                oledDrawCenterText(u8g2, rx.title, 128, 24);
                oledDrawCenterText(u8g2, rx.desc, 128, 40);
                u8g2.sendBuffer();

                vTaskDelete(NULL);
                return;
            }

            else if (strcmp(rx.title, "Startup Complete") == 0) {
                Serial1.println("Initialization Complete.  Showing OLED home screen.");

                createOLEDTask();

                vTaskDelete(NULL);
                return;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}



int frame = 0;
void tickStartupSplashScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvR08_tr);
    
    
  // Header
  u8g2.drawBox(0, 0, 128, 11);
  u8g2.setDrawColor(0);
  oledDrawCenterText(u8g2, "ProjectDMX", 128, 9);
  u8g2.setDrawColor(1);


  oledDrawCenterText(u8g2, "Starting Up...", 128, 28);



  // Draw spinner loader
  float rotation = (frame * 0.3);  // Rotation speed
  oledDrawSpinner(u8g2, 64, 48, 10, rotation);
  
  frame = (frame + 1) % 360;  // Keep frame bounded to prevent overflow


  u8g2.sendBuffer();


}