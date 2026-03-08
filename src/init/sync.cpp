#include "sync.h"

static EventGroupHandle_t gInitEvents = nullptr;

void initSyncCreate() {
  if (gInitEvents == nullptr) {
    gInitEvents = xEventGroupCreate();
  }
}

void initSyncMarkDone(EventBits_t doneBit, bool ok, EventBits_t okBit) {
  EventBits_t bits = doneBit;
  if (ok) bits |= okBit;
  xEventGroupSetBits(gInitEvents, bits);
}

bool initSyncWaitDone(EventBits_t doneBits, TickType_t timeoutTicks) {
  EventBits_t got = xEventGroupWaitBits(
      gInitEvents,
      doneBits,
      pdFALSE,   // do not clear
      pdTRUE,    // wait for all requested bits
      timeoutTicks);
  return (got & doneBits) == doneBits;
}

EventBits_t initSyncGetBits() {
  return xEventGroupGetBits(gInitEvents);
}

bool initSyncWaitOk(EventBits_t doneBit, EventBits_t okBit) {
  if (!initSyncWaitDone(doneBit, pdMS_TO_TICKS(10000))) return false;
  return (initSyncGetBits() & okBit) != 0;
}

bool initSyncDoneOk(EventBits_t doneBit, EventBits_t okBit) {
  return (initSyncGetBits() & (doneBit | okBit)) == (doneBit | okBit);
}

bool initSyncDoneError(EventBits_t doneBit, EventBits_t okBit) {
  return (initSyncGetBits() & (doneBit | okBit)) == doneBit;
}