#ifndef RTOS_CONFIG_H
#define RTOS_CONFIG_H

// ========== Task Stack Sizes (in bytes)
#define WEB_TASK_STACK_SIZE        4096
#define AUDIO_TASK_STACK_SIZE       4096
#define MDNS_TASK_STACK_SIZE        2048
#define OLED_TASK_STACK_SIZE        2048

// ========== Task Priorities
// Higher number = higher priority (max is 4 on Pico)
#define AUDIO_TASK_PRIORITY         4    // MAXIMUM - real-time audio (preempts everything)
#define WEB_TASK_PRIORITY           1    // Low - interactive
#define MDNS_TASK_PRIORITY          0    // Lowest - background service
#define OLED_TASK_PRIORITY          1    // Low - interactive

// ========== Task Timing (in milliseconds)
#define WEB_TASK_DELAY_MS           10   // Delay between client checks
#define MDNS_TASK_DELAY_MS          50   // mDNS service interval
#define AUDIO_TASK_DELAY_MS         1    // Audio processing interval
#define OLED_TASK_DELAY_MS          100  // OLED processing interval
// ========== Audio Configuration
#define DEFAULT_VOLUME              0.5f // Default audio volume (0.0 - 1.0)

#endif // RTOS_CONFIG_H
