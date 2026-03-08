#include "../../peripherals/oled/oled.h"
#include "../../peripherals/audio/audio.h"
#include "../../peripherals/ethernet/ethernet.h"

#include <Ethernet3.h>

extern void createOLEDTask();


void oledTask(void *pvParameters);