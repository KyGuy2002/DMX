# ProjectDMX Controller - FreeRTOS Organization

This project has been organized for FreeRTOS multitasking, with a modular structure that separates concerns into individual tasks and peripheral modules.

## Project Structure

```
src/
├── main.cpp                    # Main application entry point
├── pins.h                      # Hardware pin definitions
├── settings.h                  # Configuration constants
├── rtos_config.h               # FreeRTOS configuration
├── peripherals/                # Peripheral initialization modules
│   ├── peripheral_init.h       # Peripheral interfaces
│   └── peripheral_init.cpp     # Peripheral implementations
├── spi_manager/                # SPI bus mutex management
│   ├── spi_manager.h           # SPI mutex interfaces
│   └── spi_manager.cpp         # SPI mutex implementation
└── tasks/                      # FreeRTOS task modules
    ├── task_http.h/.cpp        # HTTP server task
    ├── task_audio.h/.cpp       # Audio playback task
    ├── task_mdns.h/.cpp        # mDNS discovery task
    └── task_dmx.h/.cpp         # DMX control task (placeholder)
```

## Shared Resource Management

### SPI Bus Mutex Protection

The Ethernet (W5500) and SD card modules share the same SPI bus but use different chip select (CS) pins. To prevent race conditions and data corruption, all SPI bus access is protected by a **FreeRTOS mutex**:

- **HTTP Task**: Acquires SPI mutex before accessing Ethernet
- **Audio Task**: Acquires SPI mutex before accessing SD card
- **Mutex Timeout**: Tasks use short timeouts (50-100ms) to avoid starvation

**How it works:**
```cpp
// Example: Safe SPI access
{
  SpiLock lock(pdMS_TO_TICKS(100));  // Try to acquire for 100ms
  if (lock.isAcquired()) {
    // Safe to access SPI peripherals now
    SD.open(...);  // or Ethernet operations
  }
}  // Mutex automatically released here
```

This ensures only one task accesses the SPI bus at a time, eliminating unreliable SD card reads.

The application uses multiple concurrent tasks:

1. **HTTP Server Task** (Priority 2, 4KB stack)
   - Handles incoming HTTP requests
   - Serves web interface
   - Displays system status and analog readings

2. **Audio Playback Task** (Priority 3, 8KB stack)
   - Manages MP3 file playback
   - Handles I2S audio streaming
   - Supports volume control and file switching

3. **mDNS Task** (Priority 1, 2KB stack)
   - Provides network discovery via mDNS
   - Allows access via hostname.local

4. **DMX Task** (Disabled, placeholder)
   - Reserved for future DMX512 functionality
   - Pin setup available in pins.h

5. **Idle Task** (System priority, runs loop())
   - Prints periodic system statistics
   - Low-priority background operations

### Peripheral Modules

The `peripherals/` directory contains initialization code for:
- **SPI Bus**: Shared by SD card and Ethernet
- **Ethernet**: W5500-based networking
- **SD Card**: File storage for audio files
- **I2S Audio**: Audio output to DAC

### Task Priorities

Higher number = higher priority:
- 3: Audio Playback (real-time audio processing)
- 2: HTTP Server, DMX (interactive tasks)
- 1: mDNS (background service)
- 0: Idle task (system stats)

## Configuration

### pins.h
Defines all hardware pin assignments for:
- SPI (SD + Ethernet)
- I2C (OLED)
- DMX (TX/RX/Mode)
- I2S Audio (BCK/WS/DIN)

### settings.h
Contains application settings:
- MAC address
- Audio file path
- Network configuration
- Task parameters (can be added)

## Building and Uploading

```bash
# Build the project
pio run

# Upload to device
pio run --target upload

# Monitor serial output
pio device monitor
```

## Adding New Tasks

To add a new FreeRTOS task:

1. Create `tasks/task_name.h` and `tasks/task_name.cpp`
2. Define task function: `void taskName(void *pvParameters)`
3. Define task handle: `TaskHandle_t taskNameHandle`
4. Implement `createTaskName()` using `xTaskCreate()`
5. Call `createTaskName()` in `main.cpp` setup()

Example:
```cpp
void createMyTask() {
  xTaskCreate(
    myTaskFunction,           // Task function
    "My Task",                // Name
    2048,                     // Stack size (bytes)
    NULL,                     // Parameters
    2,                        // Priority
    &myTaskHandle             // Handle
  );
}
```

## Task Communication

For inter-task communication, use:
- **Queues**: `xQueueCreate()`, `xQueueSend()`, `xQueueReceive()`
- **Semaphores**: `xSemaphoreCreateBinary()`, `xSemaphoreGive()`, `xSemaphoreTake()`
- **Mutexes**: `xSemaphoreCreateMutex()` for protecting shared resources
- **Event Groups**: `xEventGroupCreate()` for signaling events

### SPI Bus Protection

The SPI mutex (`spi_manager.h`) protects access to the shared SPI bus. Use the `SpiLock` helper class:

```cpp
#include "spi_manager/spi_manager.h"

// Automatic mutex management
{
  SpiLock lock;  // Acquire indefinitely
  // Safe to access Ethernet or SD card here
}  // Released automatically

// With timeout
{
  SpiLock lock(pdMS_TO_TICKS(100));
  if (lock.isAcquired()) {
    // Access SPI peripherals
  }
}  // Released automatically
```

Or use manual acquire/release:
```cpp
if (acquireSpiMutex()) {
  // Access SPI peripherals
  releaseSpiMutex();
} else {
  // Failed to acquire
}
```

## Debugging

Monitor output on Serial1 (115200 baud) for:
- Task startup messages
- Peripheral initialization status
- Periodic system statistics (every 30 seconds)
- Task-specific debug messages (tagged with [Task Name])

## Performance Monitoring

The idle task prints system statistics every 30 seconds:
- Uptime
- Free heap memory

To monitor individual task stack usage, add FreeRTOS stack checking:
```cpp
UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(taskHandle);
```

## Notes

- FreeRTOS scheduler is started automatically by the Arduino framework
- `delay()` calls are replaced with `vTaskDelay(pdMS_TO_TICKS(ms))` in tasks
- Shared resources (SPI bus, SD card) should use mutexes for thread safety
- Audio task has highest priority to prevent audio glitches
