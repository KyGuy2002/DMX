#include "utils.h"

// Max: 255
void writeUInt8(File &file, uint8_t value) {
  file.write((uint8_t *)&value, 1);
}

// Max: 65,535
void writeUInt16(File &file, uint16_t value) {
  file.write((uint8_t *)&value, 2);
}

// Max: 4,294,967,295
void writeUInt32(File &file, uint32_t value) {
  file.write((uint8_t *)&value, 4);
}

// Max: 18,446,744,073,709,551,615
void writeUInt64(File &file, uint64_t value) {
  file.write((uint8_t *)&value, 8);
}


/**
 * Format:
 *  - Change count: 2 bytes (uint16_t)
 *  - For each change:
 *      - Universe: 1 byte (uint8_t)
 *      - Channel: 2 bytes (uint16_t)
 *      - Value: 1 byte (uint8_t)
 */
void writeDMXFrame(File &file, uint8_t **prev, uint8_t **curr, uint16_t universeCount) {
  const size_t MAX_CHANGES = universeCount * 512;  // worst case
  const size_t MAX_BUF_SIZE = MAX_CHANGES * 4;     // 1B universe + 2B channel + 1B value

  uint8_t *buf = new uint8_t[MAX_BUF_SIZE];
  size_t bufIndex = 0;
  uint16_t changeCount = 0;

  for (uint16_t u = 0; u < universeCount; u++) {
    for (uint16_t ch = 0; ch < 512; ch++) {
      if (curr[u][ch] != prev[u][ch]) {
        // Write universe
        buf[bufIndex++] = (uint8_t)u;

        // Write channel
        buf[bufIndex++] = (uint8_t)(ch & 0xFF);
        buf[bufIndex++] = (uint8_t)(ch >> 8);

        // Write value
        buf[bufIndex++] = curr[u][ch];

        // Update prev
        prev[u][ch] = curr[u][ch];
        changeCount++;
      }
    }
  }

  // Write change count
  file.write((uint8_t *)&changeCount, sizeof(changeCount));

  // Write buffer of changes
  file.write(buf, bufIndex);

  delete[] buf;
}


// Helper to allocate a 2D DMX array
uint8_t **allocDMXArray(uint16_t universeCount) {
  uint8_t **arr = new uint8_t*[universeCount];
  for (uint16_t u = 0; u < universeCount; u++) {
    arr[u] = new uint8_t[512]();
  }
  return arr;
}

// Helper to free 2D DMX array
void freeDMXArray(uint8_t **arr, uint16_t universeCount) {
  for (uint16_t u = 0; u < universeCount; u++) {
    delete[] arr[u];
  }
  delete[] arr;
}