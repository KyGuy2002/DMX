#include "utils.h"


void oledDrawCenterText(U8G2 &u8g2, const char *text, int x, int y) {
  int w = u8g2.getStrWidth(text);
  int x1 = (x - w) / 2;
  u8g2.setCursor(x1, y);
  u8g2.print(text);
}


void oledDrawSpinner(U8G2 &u8g2, int centerX, int centerY, int radius, float angleOffset) {
  // Draw a circular spinner with a gap (like a loading circle)
  // The gap is about 60 degrees, arc is 300 degrees
  float gapSize = 60.0 * PI / 180.0;  // 60 degrees in radians
  float arcLength = (360.0 - 60.0) * PI / 180.0;  // 300 degrees
  
  int thickness = 3;  // Thickness of the spinner
  
  // Draw the arc
  for (float angle = 0; angle < arcLength; angle += 0.05) {
    float currentAngle = angle + angleOffset;
    
    // Draw outer edge
    int x1 = centerX + cos(currentAngle) * radius;
    int y1 = centerY + sin(currentAngle) * radius;
    u8g2.drawPixel(x1, y1);
    
    // Draw thickness
    for (int t = 1; t < thickness; t++) {
      int x2 = centerX + cos(currentAngle) * (radius - t);
      int y2 = centerY + sin(currentAngle) * (radius - t);
      u8g2.drawPixel(x2, y2);
    }
  }
  
  // Draw rounded caps at both ends of the arc
  float startAngle = angleOffset;
  float endAngle = angleOffset + arcLength;
  
  // Start cap (circular dot)
  int capX1 = centerX + cos(startAngle) * (radius - thickness/2);
  int capY1 = centerY + sin(startAngle) * (radius - thickness/2);
  u8g2.drawDisc(capX1, capY1, thickness/2);
  
  // End cap (circular dot)
  int capX2 = centerX + cos(endAngle) * (radius - thickness/2);
  int capY2 = centerY + sin(endAngle) * (radius - thickness/2);
  u8g2.drawDisc(capX2, capY2, thickness/2);
}