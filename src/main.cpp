/*
 * ProjectDMX Controller - FreeRTOS Implementation
 * 
 * This firmware uses FreeRTOS to manage multiple concurrent tasks:
 * - HTTP Server: Handles web interface
 * - Audio Playback: Manages MP3 playback via I2S
 * - mDNS: Provides network discovery
 * - DMX: (Future) DMX512 control
 */

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// Peripheral initialization
#include "init/init.h"


void setup(void) {
  // ========== Serial Init
  Serial1.begin(115200);
  Serial1.println();
  Serial1.println();
  Serial1.println("======================================");
  Serial1.println("  ProjectDMX Controller - FreeRTOS");
  Serial1.println("======================================");
  Serial1.println();


  initPeripherals();


 

  // Note: FreeRTOS scheduler is started automatically by Arduino framework
  // The loop() function will run as the idle task
}

void loop() {

  // Small delay to prevent idle task from consuming too much CPU
  vTaskDelay(pdMS_TO_TICKS(1000));
  
}


