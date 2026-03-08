#pragma once

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>

struct OledMsg {
	char title[30];
	char desc[30];
};

extern QueueHandle_t g_oledQueue;

void initPeripherals();