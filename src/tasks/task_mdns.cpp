#include "task_mdns.h"
#include "../peripherals/peripheral_init.h"
#include "../rtos_config.h"

TaskHandle_t mdnsTaskHandle = NULL;

void mdnsTask(void *pvParameters) {
  Serial1.println("[MDNS Task] Started");
  
  while (1) {
    // Run mDNS service
    mdns.run();
    
    // Run at moderate frequency
    vTaskDelay(pdMS_TO_TICKS(MDNS_TASK_DELAY_MS));
  }
}

void createMdnsTask() {
  xTaskCreate(
    mdnsTask,                 // Task function
    "mDNS",                   // Task name
    MDNS_TASK_STACK_SIZE,     // Stack size (bytes)
    NULL,                     // Parameters
    MDNS_TASK_PRIORITY,       // Priority (low)
    &mdnsTaskHandle           // Task handle
  );
}
