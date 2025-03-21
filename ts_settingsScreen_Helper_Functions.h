#include "ts_globalValues.h"

#ifndef SETTINGS_SCREEN
#define SETTINGS_SCREEN

void drawSettingsButtons();

void drawSettingsNumpad();

void drawSettingsScreen();

void drawChangingBox(changingParameter parameter, uint32_t newValue);

void changeStartFreq();

void changeStopFreq();

void changeStepSize();

void changeDuration();

void changeSweeps();

#endif