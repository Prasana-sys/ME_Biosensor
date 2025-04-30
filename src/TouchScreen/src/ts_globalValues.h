#include "Adafruit_RA8875.h"
#include <string>
#include "em_cmu.h"
#include "em_msc.h"

#ifndef GLOBAL_VALUES
#define GLOBAL_VALUES

#define EXPO_BACKUP_DEMO_MODE false

// RA8875 Library only supports hardware SPI at this time

/* ************************************************************************************** 

For xG24 explorer kit, there is currently a bug in Si Labs Arduino Core Version 2.3.0.
The Serial1 pins (Tx1 and Rx1) are connected to INT and PWM pins on the board respectively, 
instead of the pins which are marked as TX and RX.

A bug report was sent and is currently being investigated.
https://github.com/SiliconLabs/arduino/issues/123

Also look at /doc/xG24_pinout_bug.md

In future core versions, if this bug were to be fixed, this pinout should change.
Proposed line change:

#define RA8875_INT 13 // INT on xG24

// 11 - Tx, 12- Rx

***************************************************************************************** */
#define RA8875_INT 11 // MikroBUS-TX on xG24
#define RA8875_CS 4
#define RA8875_RESET 9 // Connected to SDA pin currently

#define STM_RES 10 // SCL

// 13 - Tx, 14- Rx

// Reserving last 799,928 bytes (99,991 * 8)
// padding ringdownData to 8 bytes (instead of orig 5 bytes) to align with word size (word size = 4 bytes)
#define RINGDOWNDATA_START_ADDRESS ((uint32_t*)0x080BCB48UL)
#define RINGDOWNDATA_END_ADDRESS ((uint32_t*)FLASH_MEM_END) // 0x0817FFFFUL

// Total Ringdowndata size: (0x0817_FFFF) - (0x080B_CB48) + 1 = C34B8 (799,928 bytes)

// Flash Page size = 0x2000 (8192 Bytes)

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
extern bool ts_ringdown_parameters_changed;

typedef struct {
    uint32_t minValue; // min and max value need to be given in relation to the smallest unit
    uint32_t maxValue; // Example: 10 MHz needs to stored as 10000000 in maxValue
    const char* minValueText;
    const char* maxValueText;
} rangeOfParameter;

constexpr rangeOfParameter parameterRanges[5] = {{.minValue = 1000, .maxValue = 10000000, .minValueText = "1 kHz", .maxValueText = "10 MHz"},
                                    {.minValue = 1000, .maxValue = 10000000, .minValueText = "1 kHz", .maxValueText = "10 MHz"},
                                    {.minValue = 10, .maxValue = 100000, .minValueText = "10 Hz", .maxValueText = "100 kHz"},
                                    {.minValue = 0, .maxValue = 1000, .minValueText = "0 ms", .maxValueText = "1000 ms"},
                                    {.minValue = 1, .maxValue = 10, .minValueText = "1", .maxValueText = "10"}};

void check_parameters_validity (bool onlyLimitCheck = false);

// Settings dimensions
// isSettingsScreen to check if touch input was on settings screen or Graph Screen.
extern bool isSettingsScreen;
const uint16_t sParameterButtonY[5] = {20, 110, 200, 290, 380};
const uint16_t sParameterButtonX = 20;
const uint16_t sParameterButtonW = 200;
const uint16_t sParameterButtonH = 80;
const uint16_t sDoneButtonX = 340;
const uint16_t sDoneButtonY = 410;
const uint16_t sDoneButtonW = 120;
const uint16_t sDoneButtonH = 60;

// Numpad dimensions
extern bool isChangingValue;
const uint16_t sNumpadButtonW = 70;
const uint16_t sNumpadButtonH = 70;
const uint16_t sNumpadButtonStartX = 540;
const uint16_t sNumpadButtonStartY = 80;
const uint16_t sNumpadButtonSpacing = 10;
// A 3x4 keypad layout (digits + Clear + OK)
extern const char* keypadLabels[12];

extern uint32_t newValue;
enum class changingParameter {None = -1, Start_Freq, Stop_Freq, Step_Size, Duration, Sweeps};
extern changingParameter selectedParameter;

typedef struct {
    uint32_t xMin, xMax; // Frequency
    uint8_t yMin, yMax; // Duration
} XYMinMax;

// Data for ringdown graph
constexpr int maxNumPoints = ((parameterRanges[1].maxValue - parameterRanges[0].minValue)/parameterRanges[2].minValue) + 1; // 99,991
extern int numPoints;
extern XYMinMax _xyMinMax;

#endif