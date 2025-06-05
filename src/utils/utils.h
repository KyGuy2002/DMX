#include <SD.h>

void writeUInt8(File &file, uint8_t value);
void writeUInt16(File &file, uint16_t value);
bool readUInt8(File &file, uint8_t &value);
bool readUInt16(File &file, uint16_t &value);

void writeDMXFrame(File &file, uint8_t **prev, uint8_t **curr, uint8_t universeCount);
bool readDMXFrameDelta(File &file, uint8_t **currentState, uint8_t **output, uint8_t universeCount);
uint8_t **allocDMXArray(uint8_t universeCount);
void freeDMXArray(uint8_t **arr, uint8_t universeCount);

void writeFileHeader(File &file, const String &name, uint8_t universeCount);