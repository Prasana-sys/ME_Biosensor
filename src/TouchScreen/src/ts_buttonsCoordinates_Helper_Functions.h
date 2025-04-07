#include "ts_buttonActions.h"
#include "ts_globalValues.h"

#ifndef BUTTON_COORDINATES
#define BUTTON_COORDINATES

typedef void (*ButtonAction)(void);

struct Button {
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
  ButtonAction action;
};

// Helper function to check if a point (tx,ty) is inside a button's rectangle.
bool isPointInsideButton(int32_t tx, int32_t ty, const Button &btn);

extern const Button mainButtons[8];

extern const Button settingsButtons[18];

#endif