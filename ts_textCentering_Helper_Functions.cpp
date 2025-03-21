#include "ts_textCentering_Helper_Functions.h"

void drawCenteredText(Adafruit_RA8875 &tft,
                      int x, int y, int w, int h,
                      const char* text,
                      uint8_t enlarge)
{
  tft.textMode();              // Switch to text mode
  tft.textEnlarge(enlarge);    // Set text scaling

  // Each char is (8*(enlarge+1)) wide, (16*(enlarge+1)) tall
  int factor = enlarge + 1;
  int charW = 8 * factor;
  int charH = 16 * factor;

  // Compute text width/height for a single-line string
  int len = strlen(text);
  int textWidth  = charW * len;
  int textHeight = charH;

  // Center the text within the box
  int textX = x + (w - textWidth) / 2;
  int textY = y + (h - textHeight) / 2;

  // Move cursor
  tft.textSetCursor(textX, textY);

  tft.textTransparent(RA8875_BLACK);

  // Write the text
  tft.textWrite(text);

  // Switch back to graphics
  tft.graphicsMode();
}

void drawCenteredMultiLine(Adafruit_RA8875 &tft,
                           int x, int y, int w, int h,
                           const char* lines[],
                           int lineCount,
                           uint8_t enlarge)
{
  // Switch to text mode
  tft.textMode();
  tft.textEnlarge(enlarge);
  tft.textTransparent(RA8875_BLACK);

  // Each character is 8*(enlarge+1) wide and 16*(enlarge+1) tall
  int factor = enlarge + 1;
  int charW  = 8  * factor;
  int charH  = 16 * factor;

  // Total height of all lines stacked with no extra spacing
  int totalHeight = lineCount * charH;

  // Y-position of the top line so that the entire block is centered
  int startY = y + (h - totalHeight) / 2;

  // Print each line
  for (int i = 0; i < lineCount; i++) {
    int len        = strlen(lines[i]);
    int textWidth  = len * charW;
    int lineX      = x + (w - textWidth) / 2;   // center horizontally
    int lineY      = startY + i * charH;        // stack vertically

    tft.textSetCursor(lineX, lineY);
    tft.textWrite(lines[i]);
  }

  // Return to graphics mode
  tft.graphicsMode();
}