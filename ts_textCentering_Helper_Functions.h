#include "Adafruit_RA8875.h"

#ifndef textCentering_Helper_Functions
#define textCentering_Helper_Functions

void drawCenteredText(Adafruit_RA8875 &tft,
                      int x, int y, int w, int h,
                      const char* text,
                      uint8_t enlarge = 0);

void drawCenteredMultiLine(Adafruit_RA8875 &tft,
                           int x, int y, int w, int h,
                           const char* lines[],
                           int lineCount,
                           uint8_t enlarge = 0);

#endif