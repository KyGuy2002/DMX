#include "rfid_task.h"


WIEGAND wg;



void createRfidTask() {

  Serial1.println("Rfid task created.");

  wg.begin(MODULE_B_PIN_1, MODULE_B_PIN_2);


  xTaskCreate(
    rfidTask,        // Task function
    "Rfid",         // Task name
    WEB_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    1,      // Priority
    NULL
  );
  
}


void rfidTask(void *pvParameters) {
  while (1) {

    if(wg.available())
    {
      Serial1.print("Wiegand HEX = ");
      Serial1.print(wg.getCode(),HEX);
      Serial1.print(", DECIMAL = ");
      Serial1.print(wg.getCode());
      Serial1.print(", Type W");
      Serial1.println(wg.getWiegandType());    
    }

    vTaskDelay(pdMS_TO_TICKS(100));
    
  }
}