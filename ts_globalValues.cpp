#include "ts_globalValues.h"

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

uint32_t parametersRingdown[5] = {100000, 1000000, 10000, 100, 1};

bool ts_ringdown_parameters_changed = false;

bool isSettingsScreen = false;
bool isChangingValue = false;
uint16_t tx = 0 , ty = 0;

const char* buttonLabels[8] = {"Start Freq", "Stop Freq", "Step Size", "Duration", "Sweeps", "Terminate", "Reset", "Start"};
const char* keypadLabels[12] = {
  "1","2","3",
  "4","5","6",
  "7","8","9",
  "C","0","OK"
};

uint32_t newValue = 0;
changingParameter selectedParameter = changingParameter::None;

// int dataX[numPoints] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 
//                         20, 22, 24, 26, 28, 30, 32, 34, 36, 38,
//                         40, 42, 44, 46, 48, 50, 52, 54, 56, 58,
//                         60, 62, 64, 66, 68, 70, 72, 74, 76, 78,
//                         80, 82, 84, 86, 88, 90, 92, 94, 96, 98 };

// int dataY[numPoints] = { // Rise from 30 to 100
//                         30, 33, 36, 39, 42, 45, 48, 50, 53, 56,
//                         59, 62, 65, 68, 71, 74, 77, 80, 82, 85,
//                         88, 91, 94, 97, 100,
//                         // Fall back down to 30
//                         100, 97, 94, 91, 88, 85, 82, 80, 77, 74,
//                         71, 68, 65, 62, 59, 56, 53, 50, 48, 45,
//                         42, 39, 36, 33, 30 };

int numPoints = 0; // 41 numPoints when testing
XYMinMax _xyMinMax = {0, 0, 0, 0};

int calibrateTSPoint( tsPoint_t * displayPtr, tsPoint_t * screenPtr, const tsMatrix_t * matrixPtr )
{
  int  retValue = 0 ;

  if( matrixPtr->Divider != 0 )
  {
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) +
                      (matrixPtr->Bn * screenPtr->y) +
                       matrixPtr->Cn
                    ) / matrixPtr->Divider ;

    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) +
                      (matrixPtr->En * screenPtr->y) +
                       matrixPtr->Fn
                    ) / matrixPtr->Divider ;
  }
  else
  {
    return -1;
  }

  return( retValue );
}