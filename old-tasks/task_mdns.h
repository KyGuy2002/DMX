#ifndef TASK_MDNS_H
#define TASK_MDNS_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// Task handle
extern TaskHandle_t mdnsTaskHandle;

// Task function
void mdnsTask(void *pvParameters);

// Task creation
void createMdnsTask();

#endif // TASK_MDNS_H
