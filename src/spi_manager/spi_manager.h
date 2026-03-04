#ifndef SPI_MANAGER_H
#define SPI_MANAGER_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>

// SPI Mutex Manager
// Ensures exclusive access to the shared SPI bus
// Prevents SD card and Ethernet from interfering with each other

extern SemaphoreHandle_t spiMutex;

// Initialize the SPI mutex
void initSpiMutex();

// Acquire SPI bus (wait indefinitely)
// Returns true if successful, false on error
bool acquireSpiMutex();

// Acquire SPI bus with timeout
// Returns true if acquired, false if timeout
bool acquireSpiMutexTimed(TickType_t timeoutTicks);

// Release SPI bus
void releaseSpiMutex();

// Helper class for RAII-style mutex management
class SpiLock {
public:
  SpiLock(TickType_t timeoutTicks = portMAX_DELAY) {
    if (timeoutTicks == portMAX_DELAY) {
      acquired = acquireSpiMutex();
    } else {
      acquired = acquireSpiMutexTimed(timeoutTicks);
    }
  }
  
  ~SpiLock() {
    if (acquired) {
      releaseSpiMutex();
    }
  }
  
  bool isAcquired() const { return acquired; }
  
private:
  bool acquired;
};

#endif // SPI_MANAGER_H
