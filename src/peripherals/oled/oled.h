#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "pins.h"
#include "../../init/sync.h"
#include "../../utils/utils.h"

extern U8G2_SSD1306_128X64_NONAME_F_2ND_HW_I2C u8g2;

extern void createOLEDInitTask();

void oledInitTask(void *pvParameters);