#include "ts_buttonActions.h"
#include <WatchdogTimer.h>
#include "ts_settingsScreen_Helper_Functions.h"
#include "ts_mainScreen_Helper_Functions.h"

void doNothing () {

}

void startFreqMainButton() {
  Serial.println("Start Freq main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true; 
  changeStartFreq();
}

void stopFreqMainButton() {
  Serial.println("Start Freq main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeStopFreq(); 
}

void stepSizeMainButton() {
  Serial.println("Step Size main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeStepSize(); 
}

void durationMainButton() {
  Serial.println("Duration main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeDuration(); 
}

void sweepsMainButton() {
  Serial.println("Sweeps main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeSweeps(); 
}

void terminateMainButton() {
  Serial.println("Terminate main button pressed");
  Serial.println("Terminating ringdown process");

  // TODO: Need to handle termination
}

void resetMainButton() {
  Serial.println("Resetting Si Labs MCU and Screen");
  WatchdogTimer.begin(WDOG_PERIOD_1_S);
}

void startMainButton() {
  Serial.println("Start button pressed");

  // TODO: Send UART communication to STM to start ringdown and receive data back

  // Increment numPoints based on incoming data

  // Draw the ringdown graph using the provided data
  drawRingdownGraph();
}

void startFreqSettingsButton() {
  Serial.println("Start Freq settings button pressed");
  changeStartFreq();
}

void stopFreqSettingsButton() {
  Serial.println("Stop Freq settings button pressed");
  changeStopFreq();
}

void stepSizeSettingsButton() {
  Serial.println("Step size settings button pressed");
  changeStepSize();
}

void durationSettingsButton() {
  Serial.println("Duration settings button pressed");
  changeDuration();
}

void sweepsSettingButton() {
  Serial.println("Sweep settings button pressed");
  changeSweeps();
}

void doneSettingsButton() {
  Serial.println("Done button pressed");
  // Draw Graph/Main Screen
  drawMainScreen();
  isSettingsScreen = false;
  isChangingValue = false;
  newValue = 0;
  selectedParameter = changingParameter::None;
}

void numpad1() {
  Serial.println("1 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 1;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad2() {
  Serial.println("2 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 2;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad3() {
  Serial.println("3 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 3;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad4() {
  Serial.println("4 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 4;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad5() {
  Serial.println("5 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 5;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad6() {
  Serial.println("6 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 6;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad7() {
  Serial.println("7 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 7;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad8() {
  Serial.println("8 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 8;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad9() {
  Serial.println("9 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 9;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpadC() {
  Serial.println("C button pressed");
  if (isChangingValue){
    newValue = 0;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad0() {
  Serial.println("0 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 0;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpadOK() {
  Serial.println("OK button pressed");
  if (isChangingValue){

    // Check if newValue is in valid range of selectedParameter
    if (newValue < parameterRanges[static_cast<int>(selectedParameter)].minValue) {
      newValue = parameterRanges[static_cast<int>(selectedParameter)].minValue;
    }
    else if (newValue > parameterRanges[static_cast<int>(selectedParameter)].maxValue) {
      newValue = parameterRanges[static_cast<int>(selectedParameter)].maxValue;
    }
    
    parametersRingdown[static_cast<int>(selectedParameter)] = newValue;
    // Clear out changing box
    tft.fillRect(310, 200, sParameterButtonW, sParameterButtonH, RA8875_WHITE);

    isChangingValue = false;
    selectedParameter = changingParameter::None;
    newValue = 0;

    // Redraw Settings buttons
    drawSettingsButtons();
  }
}