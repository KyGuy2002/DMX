#include <U8g2lib.h>


extern void oledDrawCenterText(U8G2 &u8g2, const char *text, int x, int y);
extern void oledDrawSpinner(U8G2 &u8g2, int centerX, int centerY, int radius, float angleOffset);
extern uint8_t **allocDMXArray(uint16_t universeCount);
extern void freeDMXArray(uint8_t **arr, uint16_t universeCount);