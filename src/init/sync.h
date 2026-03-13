#pragma once
#include <Arduino.h>
#include <FreeRTOS.h>
#include <event_groups.h>

enum InitBit : EventBits_t {
  INIT_AUDIO_DONE = (1 << 1),
  INIT_OLED_DONE  = (1 << 2),
  INIT_ETHERNET_DONE   = (1 << 3),
  INIT_SD_DONE    = (1 << 4),
  INIT_DMX_DONE   = (1 << 5),

  INIT_AUDIO_OK   = (1 << 9),
  INIT_OLED_OK    = (1 << 10),
  INIT_ETHERNET_OK     = (1 << 11),
  INIT_SD_OK      = (1 << 12),
  INIT_DMX_OK     = (1 << 13),
};

void initSyncCreate();
void initSyncMarkDone(EventBits_t doneBit, bool ok, EventBits_t okBit);
bool initSyncWaitDone(EventBits_t doneBits, TickType_t timeoutTicks = portMAX_DELAY);
EventBits_t initSyncGetBits();

/**
 * Waits for the specified initialization step to be done, then checks if it was successful.
 * This IS A WAITING FUNCTION - it will block until the doneBit is set or the timeout expires.
 */
bool initSyncWaitOk(EventBits_t doneBit, EventBits_t okBit);

/**
 * Checks if the specified initialization step is done and successful.
 * This DOES NOT WAIT - it just checks the current bits and returns immediately.
 */
bool initSyncDoneOk(EventBits_t doneBit, EventBits_t okBit);

/**
 * Checks if the specified initialization step is done and failed.
 * This DOES NOT WAIT - it just checks the current bits and returns immediately.
 */
bool initSyncDoneError(EventBits_t doneBit, EventBits_t okBit);