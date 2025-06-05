#include <SD.h>

void writeUInt8(File &file, uint8_t value);
void writeUInt16(File &file, uint16_t value);
void writeUInt32(File &file, uint32_t value);
void writeUInt64(File &file, uint64_t value);

void writeDMXFrame(File &file, uint8_t **prev, uint8_t **curr, uint16_t universeCount);
uint8_t **allocDMXArray(uint16_t universeCount);
void freeDMXArray(uint8_t **arr, uint16_t universeCount);