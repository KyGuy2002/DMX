#include "oled_loading_task.h"
#include "../../tasks/oled/oled_task.h"


TaskHandle_t g_oledStartupSplashTaskHandle = nullptr;


void createOLEDStartupSplashTask() {
  xTaskCreate(
    oledStartupSplashTask,             // Task function
    "OLED Startup Splash",    // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    &g_oledStartupSplashTaskHandle                      // Task handle
  );
}


void oledStartupSplashTask(void *pvParameters) {
    OledMsg rx{};
    while (1) {
        tickStartupSplashScreen();

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