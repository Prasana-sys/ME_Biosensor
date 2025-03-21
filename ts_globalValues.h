#include "Adafruit_RA8875.h"

#ifndef GLOBAL_VALUES
#define GLOBAL_VALUES

// RA8875 Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 10
#define RA8875_CS 4
#define RA8875_RESET 9

extern Adafruit_RA8875 tft;
extern uint16_t tx, ty;
// Calibration Matrix
const tsMatrix_t tsMatrix = {.An = -327680, .Bn = 2240, .Cn = 10733920, 
                             .Dn = -2688, .En = -220800, .Fn = 26770272, 
                             .Divider = -392566};

int calibrateTSPoint( tsPoint_t * displayPtr, tsPoint_t * screenPtr, const tsMatrix_t * matrixPtr );

// Graph area dimensions
// Graph origin at (30, 370)
const uint16_t graphX = 20;
const uint16_t graphY = 20;
const uint16_t graphW = 480;
const uint16_t graphH = 350;

// Info Box dimensions
// Top left corner at (540, 20)
const uint16_t infoX = 540;
const uint16_t infoY = 20;
const uint16_t infoW = 240;
const uint16_t infoH = 350;

// Row of 5 parameter buttons
const uint16_t buttonY = 400;
const uint16_t pButtonX[5] = {20, 130, 240, 330, 420};
const uint16_t pButtonW[5] = {100, 100, 80, 80, 60};
const uint16_t buttonH = 60;

// Row of 3 control buttons
const uint16_t cButtonX[3] = {490, 590, 690};
const uint16_t cButtonW = 90;

// Label text for each button
extern const char* buttonLabels[8];

// Default Parameter data
// 0 = Start Frequency
// 1 = Stop Frequency
// 2 = Step Size
// 3 = Forced Response duration (ms)
// 4 = Number of sweeps
extern uint32_t parametersRingdown[5];

// Settings dimensions
// isSettingsScreen to check if touch input was on settings screen or Graph Screen.
extern bool isSettingsScreen;
const uint16_t sParameterButtonY[5] = {20, 110, 200, 290, 380};
const uint16_t sParameterButtonX = 20;
const uint16_t sParameterButtonW = 180;
const uint16_t sParameterButtonH = 80;
const uint16_t sDoneButtonX = 340;
const uint16_t sDoneButtonY = 410;
const uint16_t sDoneButtonW = 120;
const uint16_t sDoneButtonH = 60;

// Numpad dimensions
extern bool isChangingValue;
const uint16_t sNumpadButtonW = 50;
const uint16_t sNumpadButtonH = 50;
const uint16_t sNumpadButtonStartX = 580;
const uint16_t sNumpadButtonStartY = 110;
const uint16_t sNumpadButtonSpacing = 10;
// A 3x4 keypad layout (digits + Clear + OK)
extern const char* keypadLabels[12];

extern uint32_t newValue;
enum class changingParameter {None, Start_Freq, Stop_Freq, Step_Size, Duration, Sweeps};
extern changingParameter selectedParameter;

// Example data for the ringdown graph
// (Replace these with actual x and y coordinate arrays and adjust mapping ranges)
const int numPoints = 50;
extern int dataX[numPoints];
extern int dataY[numPoints];

// Define mapping ranges for data (adjust these based on actual data's min/max)
// TODO: Need to declare as extern when having actual data
const int xMin = 0, xMax = 100;
const int yMin = 0, yMax = 120;

#endif