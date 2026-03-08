#ifndef TASK_OLED_H
#define TASK_OLED_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// Task handle
extern TaskHandle_t oledTaskHandle;

// Task function
void oledTask(void *pvParameters);

// Task creation
void createOLEDTask();

#endif // TASK_OLED_H
