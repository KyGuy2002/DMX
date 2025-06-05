#include "utils.h"


const String magic = "PROJECT-DMX-DMXS"; // 16 bytes (ascii)


// Max: 255
void writeUInt8(File &file, uint8_t value) {
  file.write((uint8_t *)&value, 1);
}

// Max: 65,535
void writeUInt16(File &file, uint16_t value) {
  file.write((uint8_t *)&value, 2);
}

bool readUInt8(File &file, uint8_t &value) {
  return file.read(&value, 1) == 1;
}

bool readUInt16(File &file, uint16_t &value) {
  return file.read((uint8_t *)&value, sizeof(uint16_t)) == sizeof(uint16_t);
}


/**
 * Format:
 *  - Change count: 2 bytes (uint16_t)
 *  - For each change:
 *      - Universe: 1 byte (uint8_t)
 *      - Channel: 2 bytes (uint16_t)
 *      - Value: 1 byte (uint8_t)
 */
void writeDMXFrame(File &file, uint8_t **prev, uint8_t **curr, uint8_t universeCount) {
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


bool readDMXFrameDelta(File &file, uint8_t **currentState, uint8_t **output, uint8_t universeCount) {
  uint16_t changeCount;
  if (!readUInt16(file, changeCount)) return false;

  // Copy current state to output
  for (uint16_t u = 0; u < universeCount; u++) {
    memcpy(output[u], currentState[u], 512);
  }

  for (uint16_t i = 0; i < changeCount; i++) {
    uint8_t universe;
    uint16_t channel;
    uint8_t value;

    if (!readUInt8(file, universe)) return false;
    if (!readUInt16(file, channel)) return false;
    if (!readUInt8(file, value)) return false;

    if (universe >= universeCount || channel >= 512) return false;

    output[universe][channel] = value;
  }

  return true;
}


// Helper to allocate a 2D DMX array
uint8_t **allocDMXArray(uint8_t universeCount) {
  uint8_t **arr = new uint8_t*[universeCount];
  for (uint16_t u = 0; u < universeCount; u++) {
    arr[u] = new uint8_t[512]();
  }
  return arr;
}

// Helper to free 2D DMX array
void freeDMXArray(uint8_t **arr, uint8_t universeCount) {
  for (uint16_t u = 0; u < universeCount; u++) {
    delete[] arr[u];
  }
  delete[] arr;
}


void writeFileHeader(File &file, const String &name, uint8_t universeCount) {
  file.print(magic); // 16 bytes

  // Version: 1
  writeUInt8(file, 1);

  // Universes: 1
  writeUInt8(file, universeCount);

  // Duration seconds: 0 (Placeholder)
  writeUInt16(file, 0);

  // Reserved: 12 bytes of 0
  for (int i = 0; i < 12; ++i) {
      writeUInt8(file, 0);
  }
}