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
#include "peripherals/peripheral_init.h"
#include "init/init.cpp"

// Task modules
#include "tasks/task_http.h"
#include "tasks/task_audio.h"
#include "tasks/task_mdns.h"
#include "tasks/task_oled.h"

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


  // ========== Initialize Peripherals
  Serial1.println("Initializing peripherals...");

  if (!initOLED()) {
    Serial1.println("FATAL: OLED initialization failed!");
    while (true) delay(1000);
  }

  Serial1.println("- OLED task starting...");
  createOLEDTask();

  if (!initSPI()) {
    Serial1.println("FATAL: SPI initialization failed!");
    while (true) delay(1000);
  }

  if (!initEthernet()) {
    Serial1.println("FATAL: Ethernet initialization failed!");
    while (true) delay(1000);
  }

  if (!initSDCard()) {
    Serial1.println("FATAL: SD Card initialization failed!");
    while (true) delay(1000);
  }

  if (!initI2SAudio()) {
    Serial1.println("FATAL: I2S Audio initialization failed!");
    while (true) delay(1000);
  }

  Serial1.println();
  Serial1.println("All peripherals initialized successfully.");
  Serial1.println();

  // ========== Create FreeRTOS Tasks
  Serial1.println("Creating FreeRTOS tasks...");
  
  Serial1.println("- HTTP Server task starting...");
  createHttpTask();
  
  Serial1.println("- Audio Playback task starting...");
  createAudioTask();
  
  Serial1.println("- mDNS task starting...");
  createMdnsTask();

  Serial1.println();
  Serial1.println("======================================");
  Serial1.println("  System Ready - Tasks Running");
  Serial1.println("======================================");
  Serial1.println();

  // Note: FreeRTOS scheduler is started automatically by Arduino framework
  // The loop() function will run as the idle task
}

void loop() {
  // This runs as the idle task with lowest priority
  // Put any low-priority background tasks here
  
  // Optional: Print system stats periodically
  static unsigned long lastStats = 0;
  if (millis() - lastStats > 30000) {  // Every 30 seconds
    lastStats = millis();
    Serial1.println();
    Serial1.println("=== System Status ===");
    Serial1.print("Uptime: ");
    Serial1.print(millis() / 1000);
    Serial1.println(" seconds");
    Serial1.print("Free heap: ");
    Serial1.print(rp2040.getFreeHeap());
    Serial1.println(" bytes");
    Serial1.println("=====================");
    Serial1.println();
  }
  
  // Small delay to prevent idle task from consuming too much CPU
  vTaskDelay(pdMS_TO_TICKS(1000));
}


