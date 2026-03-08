#include <Arduino.h>
#include <U8g2lib.h>
#include "../../init/sync.h"
#include "../../utils/utils.h"
#include "../../init/init.h"
#include "oled.h"

extern TaskHandle_t g_oledStartupSplashTaskHandle;

extern void createOLEDStartupSplashTask();

void oledStartupSplashTask(void *pvParameters);
void tickStartupSplashScreen();