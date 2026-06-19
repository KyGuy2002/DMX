#include "neo_gen_task.h"


Adafruit_NeoPixel strip(NEO_C_LENGTH, MODULE_C_PIN_1, NEO_GRB + NEO_KHZ800);


void createNeoGenTask() {

  Serial1.println("Neo Gen task created.");

  strip.begin();
  strip.show();
  strip.setBrightness(100);


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

  while (1) {

    // Take a quick snapshot of the DMX frame under mutex, then release immediately.
    if (xSemaphoreTake(xDmxMutex, pdMS_TO_TICKS(20)) != pdTRUE) {
      vTaskDelay(pdMS_TO_TICKS(2));
      continue;
    }

    // Copy relevant DMX universes
    memcpy(dmxFrameSnapshot, dmxBuffer[0], 512);
    xSemaphoreGive(xDmxMutex);

    tickChase(11, NEO_C_LENGTH, tickCtr, curPxl, dmxFrameSnapshot);

    strip.show();

    // Yield briefly before preparing the next frame.
    vTaskDelay(pdMS_TO_TICKS(25));
    
  }
}

void tickChase(int startChannel, int noPixels, int& ticksSinceLastGlowWireUpdate, int& currentPix, uint8_t dmx[512]) {

  // Read params
  int dotRed = dmx[startChannel];
  int dotGreen = dmx[startChannel + 1];
  int dotBlue = dmx[startChannel + 2];
  int dotSpeed = dmx[startChannel + 3];
  int dotCount = ((dmx[startChannel + 4] / 255.0f) * 19) + 1; // Ensure at least 1 dot
  int dotWidth = dmx[startChannel + 5];
  int backRed = dmx[startChannel + 6];
  int backGreen = dmx[startChannel + 7];
  int backBlue = dmx[startChannel + 8];


  // Speed settings
  const int MAX_SKIP_TICKS = 25; // 1 sec between moves at lowest speed
  const int MAX_SKIP_PIXELS = 5; // moves 200 pixels per second at highest speed
  const int MIDPOINT = 90;

  int skipTicks = 0;
  int skipPixels = 0;
  if (dotSpeed < MIDPOINT) skipTicks = map(dotSpeed, 0, MIDPOINT - 1, MAX_SKIP_TICKS, 0);
  else if (dotSpeed > MIDPOINT) skipPixels = map(dotSpeed, MIDPOINT + 1, 254, 0, MAX_SKIP_PIXELS);

  // Slower - skip movements
  if (ticksSinceLastGlowWireUpdate++ < skipTicks) {
    return;
  }
  ticksSinceLastGlowWireUpdate = 0;

  // Faster - skip pixels
  const int spacing = (noPixels / dotCount);
  currentPix += 1 + skipPixels;
  if (currentPix >= spacing) {
    currentPix = 0;
  }

  // Width
  const int positiveCount = map(dotWidth, 0, 255, 1, spacing);

  Serial1.print("skipticks: ");
  Serial1.print(skipTicks);
  Serial1.print("   -   skippixels: ");
  Serial1.println(skipPixels);


  // Set all background color
  for (uint16_t i = 0; i < noPixels; i++) {
    strip.setPixelColor(i, strip.Color(backRed, backGreen, backBlue));
  }

  for (uint16_t i = 0; i < dotCount; i++) {
    int pix = (currentPix + i * spacing) % noPixels;
    for (int w = 0; w < positiveCount; w++) {
      strip.setPixelColor(pix + w, strip.Color(dotRed, dotGreen, dotBlue));
    }
  }

}