#include <SD.h>
#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include "../../../rtos_config.h"
#include "../../../pins.h"

void handleMcpInterrupt();

extern void createRfidTask();

void rfidTask(void *pvParameters);