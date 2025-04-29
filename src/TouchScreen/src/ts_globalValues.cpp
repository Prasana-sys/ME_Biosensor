#include "ts_globalValues.h"
#include <cmath>

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

uint32_t parametersRingdown[5] = {440000, 450000, 250, 100, 1};

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

int numPoints = 0;
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

// Checks if parameter values are within valid ranges
// Checks if number of points is less than maximum number
// Current maximum = 2000
void check_parameters_validity(bool onlyLimitCheck) {

  if (!onlyLimitCheck){
    for (uint8_t i = 0; i < 5; i++){
      if (parametersRingdown[i] < parameterRanges[i].minValue) {
        parametersRingdown[i] = parameterRanges[i].minValue;
      }
      else if (parametersRingdown[i] > parameterRanges[i].maxValue) {
        parametersRingdown[i] = parameterRanges[i].maxValue;
      }
    }
  }

  // Start Freq > Stop Freq ? Swap them around
  if (parametersRingdown[0] > parametersRingdown[1]) {
    uint32_t temp = parametersRingdown[1];
    parametersRingdown[1] = parametersRingdown[0];
    parametersRingdown[0] = temp;
  }

  int expectedNumberOfPoints = ((parametersRingdown[1] - parametersRingdown[0])/parametersRingdown[2]) + 1;

  if (expectedNumberOfPoints > maxNumPoints){
    // Adjust step size to accomodate for frequency span and maxNumPoints
    uint32_t newStepSize = ceil((double)(parametersRingdown[1] - parametersRingdown[0])/(maxNumPoints-1));
    if (newStepSize < parameterRanges[2].minValue) {
      newStepSize = parameterRanges[2].minValue;
    }
    else if (newStepSize > parameterRanges[2].maxValue) {
      newStepSize = parameterRanges[2].maxValue;
    }
    parametersRingdown[2] = newStepSize;
  }
}