#include "oled_task.h"

static bool g_volumeButtonsInitialized = false;
static TaskHandle_t g_oledTaskHandle = NULL;
static volatile uint32_t g_upInterruptCount = 0;
static volatile uint32_t g_downInterruptCount = 0;
static volatile TickType_t g_lastUpInterruptTick = 0;
static volatile TickType_t g_lastDownInterruptTick = 0;

static const uint32_t EVENT_VOLUME_UP = (1UL << 0);
static const uint32_t EVENT_VOLUME_DOWN = (1UL << 1);
static const TickType_t BUTTON_DEBOUNCE_TICKS = pdMS_TO_TICKS(200);


char lastModalText[30] = "";
int lastModalAgeTicks = 999;


void onVolumeUpPressed() {
  Serial1.println("========= Volume Up Pressed");
  
  if (volume.volume() < 1.0) volume.setVolume(volume.volume() + 0.1);

  lastModalAgeTicks = 0;
  strcpy(lastModalText, "Volume Up");

}

void onVolumeDownPressed() {
  Serial1.println("========= Volume Down Pressed");
  
  if (volume.volume() > 0) volume.setVolume(volume.volume() - 0.1);

  lastModalAgeTicks = 0;
  strcpy(lastModalText, "Volume Down");

}

void onVolumeUpInterrupt() {
  const TickType_t now = xTaskGetTickCountFromISR();
  if ((g_lastUpInterruptTick != 0) && ((now - g_lastUpInterruptTick) < BUTTON_DEBOUNCE_TICKS)) {
    return;
  }
  g_lastUpInterruptTick = now;

  g_upInterruptCount++;

  if (g_oledTaskHandle == NULL) {
    return;
  }

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xTaskNotifyFromISR(g_oledTaskHandle, EVENT_VOLUME_UP, eSetBits, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void onVolumeDownInterrupt() {
  const TickType_t now = xTaskGetTickCountFromISR();
  if ((g_lastDownInterruptTick != 0) && ((now - g_lastDownInterruptTick) < BUTTON_DEBOUNCE_TICKS)) {
    return;
  }
  g_lastDownInterruptTick = now;

  g_downInterruptCount++;

  if (g_oledTaskHandle == NULL) {
    return;
  }

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xTaskNotifyFromISR(g_oledTaskHandle, EVENT_VOLUME_DOWN, eSetBits, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void initVolumeButtonInterrupts() {
  if (g_volumeButtonsInitialized) {
    return;
  }

  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_UP_PIN), onVolumeUpInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN_PIN), onVolumeDownInterrupt, FALLING);

  g_volumeButtonsInitialized = true;
}

static void processVolumeButtonEvents(uint32_t eventMask) {
  if ((eventMask & EVENT_VOLUME_UP) != 0) {
    onVolumeUpPressed();
  }

  if ((eventMask & EVENT_VOLUME_DOWN) != 0) {
    onVolumeDownPressed();
  }
}


void createOLEDTask() {
  xTaskCreate(
    oledTask,             // Task function
    "OLED Task",    // Task name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    3,                        // Priority
    &g_oledTaskHandle         // Task handle
  );
}




void oledTask(void *pvParameters) {
  initVolumeButtonInterrupts();



  while (1) {
    uint32_t eventMask = 0;






    // Show modal
    u8g2.clearBuffer();
    if (lastModalAgeTicks < 8) {

      // Modal label
      u8g2.setDrawColor(1);
      oledDrawCenterText(u8g2, lastModalText, 128, 20);

      // Modal background box
      u8g2.drawBox(16, 22, 128-32, 64-44);


      // Progress bar void
      u8g2.setDrawColor(0);
      int wallThick = 2;
      u8g2.drawBox(16+wallThick, 22+wallThick, 128-(16+wallThick)*2, 64-(22+wallThick)*2);


      // Progress bar
      u8g2.setDrawColor(1);
      int barSections = 10;
      int filledSections = volume.volume() * 10;

      int left = 16+(wallThick*2);
      int maxWidth = 128 - ((4*wallThick) + 32);
      int secWidth = maxWidth / barSections;
      int finalWidth = secWidth * filledSections;

      u8g2.drawBox(left, 22+(wallThick*2), finalWidth, 64-(22+(wallThick*2))*2);

      lastModalAgeTicks++;
    }


    // Regular dash stuff
    else {
      
      oledDrawCenterText(u8g2, "ProjectDMX Home", 128, 20);
      // oledDrawCenterText(u8g2, Ethernet.localIP().toString().c_str(), 128, 45);
      oledDrawCenterText(u8g2, MDNS_NAME, 128, 56);

    }
    u8g2.sendBuffer();




    // Wake on ISR events, or timeout periodically for pin/IRQ diagnostics.
    if (xTaskNotifyWait(0, 0xFFFFFFFF, &eventMask, pdMS_TO_TICKS(100)) == pdTRUE) {
      processVolumeButtonEvents(eventMask);
    }
  }
}