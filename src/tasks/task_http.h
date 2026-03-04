#ifndef TASK_HTTP_H
#define TASK_HTTP_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// Task handle
extern TaskHandle_t httpTaskHandle;

// Task function
void httpServerTask(void *pvParameters);

// Task creation
void createHttpTask();

#endif // TASK_HTTP_H
