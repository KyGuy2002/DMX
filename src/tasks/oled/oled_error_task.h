#include "../../peripherals/oled/oled.h"



extern void createOLEDErrorTask(char *title, char *desc);

void oledErrorTask(void *pvParameters);