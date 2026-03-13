#include "artnet_task.h"


ArtnetReceiver artnet;



void createArtnetTask() {


  char SERVICE_NAME[30];
  snprintf(SERVICE_NAME, sizeof(SERVICE_NAME), "ProjectDMX %02x%02x", MAC_ADDRESS[4], MAC_ADDRESS[5]);

  artnet.setArtPollReplyConfigLongName(SERVICE_NAME);
  artnet.setArtPollReplyConfigShortName(SERVICE_NAME);
  artnet.begin();


  Serial1.println("Artnet task created.");
  

  xTaskCreate(
    artnetTask,        // Task function
    "Artnet",         // Task name
    AUDIO_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    2,      // Priority (higher for audio)
    NULL
  );
  
}


void artnetTask(void *pvParameters) {
  while (1) {


    


    // Briefly block to allow lower-priority networking tasks to run
    vTaskDelay(pdMS_TO_TICKS(100));
    
  }
}