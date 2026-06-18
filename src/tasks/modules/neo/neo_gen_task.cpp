#include "neo_gen_task.h"


CRGB leds[NEO_C_LENGTH];


void createNeoGenTask() {

  Serial1.println("Neo Gen task created.");

  FastLED.addLeds<WS2812B, MODULE_C_PIN_2, GRB>(leds, NEO_C_LENGTH);
  Serial1.println("================= 0");
  FastLED.setBrightness(100);
  Serial1.println("================= 1");

  FastLED.showColor(CRGB::Black);
  Serial1.println("================= 2");
  FastLED.show();

  Serial1.println("================= a starting actual task...");


  xTaskCreate(
    neoGenTask,        // Task function
    "Neo Gen",         // Task name
    WEB_TASK_STACK_SIZE / sizeof(StackType_t), // Stack size (words; bytes configured in rtos_config)
    NULL,                     // Parameters
    1,      // Priority
    NULL
  );
  
}

int tickCtr = 0;
int curPxl = 0;
void neoGenTask(void *pvParameters) {
  static uint8_t dmxFrameSnapshot[512];
  Serial1.println("================= b");

  while (1) {
    Serial1.println("================= c");

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    // Copy relevant DMX universes
    memcpy(dmxFrameSnapshot, dmxBuffer[0], 512);
    xSemaphoreGive(xDmxMutex);

    tickChase(11, NEO_C_LENGTH, tickCtr, curPxl, dmxFrameSnapshot);

    FastLED.show();

    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(25));
    
  }
}

void tickChase(int startChannel, int noPixels, int& ticksSinceLastGlowWireUpdate, int& currentPix, uint8_t dmx[512]) {

  // Speed
  // int ticksBetween = ((1 - (dmx[startChannel + 3] / 255.0f)) * 5) + 0.01; // Speed from 0 ticks to 15 ticks between updates
  int ticksBetween = 1;
  if (ticksSinceLastGlowWireUpdate < ticksBetween) {
    ticksSinceLastGlowWireUpdate++;
    return;
  }
  ticksSinceLastGlowWireUpdate = 0;

  // Read params
  int dotRed = dmx[startChannel];
  int dotGreen = dmx[startChannel + 1];
  int dotBlue = dmx[startChannel + 2];
  int dotCount = ((dmx[startChannel + 4] / 255.0f) * 19) + 1; // Ensure at least 1 dot
  int backRed = dmx[startChannel + 5];
  int backGreen = dmx[startChannel + 6];
  int backBlue = dmx[startChannel + 7];

  const int spacing = (200 / dotCount);

  // Set all background color
  for (uint16_t i = 0; i < 200; i++) {
    leds[i] = CRGB(backRed, backGreen, backBlue);
  }

  for (uint16_t i = 0; i < dotCount; i++) {
    int pix = (currentPix + i * spacing) % 200;
    leds[pix] = CRGB(dotRed, dotGreen, dotBlue);
  }

  if (currentPix++ >= spacing) {
    currentPix = 0;
  }

}