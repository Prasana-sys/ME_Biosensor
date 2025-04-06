#include "ts_buttonsCoordinates_Helper_Functions.h"

// Helper function to check if a point (tx,ty) is inside a button's rectangle.
bool isPointInsideButton(int32_t tx, int32_t ty, const Button &btn) {
    return (tx >= btn.x && tx <= (btn.x + btn.w) &&
            ty >= btn.y && ty <= (btn.y + btn.h));
}

const Button mainButtons[8] = {
    // Start Freq
    {pButtonX[0], buttonY, pButtonW[0], buttonH, startFreqMainButton},
    // Stop Freq
    {pButtonX[1], buttonY, pButtonW[1], buttonH, stopFreqMainButton},
    // Step Size
    {pButtonX[2], buttonY, pButtonW[2], buttonH, stepSizeMainButton},
    // Duration
    {pButtonX[3], buttonY, pButtonW[3], buttonH, durationMainButton},
    // Sweeps
    {pButtonX[4], buttonY, pButtonW[4], buttonH, sweepsMainButton},
    // Terminate
    {cButtonX[0], buttonY, cButtonW, buttonH, terminateMainButton},
    // {cButtonX[0], buttonY, cButtonW, buttonH, doNothing},
    // Reset
    {cButtonX[1], buttonY, cButtonW, buttonH, resetMainButton},
    // Start
    {cButtonX[2], buttonY, cButtonW, buttonH, startMainButton}
};

const Button settingsButtons[18] = {
    // Start Freq
    {sParameterButtonX, sParameterButtonY[0], sParameterButtonW, sParameterButtonH, startFreqSettingsButton},
    // Stop Freq
    {sParameterButtonX, sParameterButtonY[1], sParameterButtonW, sParameterButtonH, stopFreqSettingsButton},
    // Step Size
    {sParameterButtonX, sParameterButtonY[2], sParameterButtonW, sParameterButtonH, stepSizeSettingsButton},
    // Duration
    {sParameterButtonX, sParameterButtonY[3], sParameterButtonW, sParameterButtonH, durationSettingsButton},
    // Sweeps
    {sParameterButtonX, sParameterButtonY[4], sParameterButtonW, sParameterButtonH, sweepsSettingButton},
    // Done
    {sDoneButtonX, sDoneButtonY, sDoneButtonW, sDoneButtonH, doneSettingsButton},
    // Numpad 1
    {sNumpadButtonStartX, sNumpadButtonStartY, sNumpadButtonW, sNumpadButtonH, numpad1},
    // Numpad 2
    {sNumpadButtonStartX + sNumpadButtonW + sNumpadButtonSpacing, sNumpadButtonStartY, sNumpadButtonW, sNumpadButtonH, numpad2},
    // Numpad 3
    {sNumpadButtonStartX + 2 * (sNumpadButtonW + sNumpadButtonSpacing), sNumpadButtonStartY, sNumpadButtonW, sNumpadButtonH, numpad3},
    // Numpad 4
    {sNumpadButtonStartX, sNumpadButtonStartY + sNumpadButtonH + sNumpadButtonSpacing, sNumpadButtonW, sNumpadButtonH, numpad4},
    // Numpad 5
    {sNumpadButtonStartX + sNumpadButtonW + sNumpadButtonSpacing, sNumpadButtonStartY + sNumpadButtonH + sNumpadButtonSpacing, sNumpadButtonW, sNumpadButtonH, numpad5},
    // Numpad 6
    {sNumpadButtonStartX + 2 * (sNumpadButtonW + sNumpadButtonSpacing), sNumpadButtonStartY + sNumpadButtonH + sNumpadButtonSpacing, sNumpadButtonW, sNumpadButtonH, numpad6},
    // Numpad 7
    {sNumpadButtonStartX, sNumpadButtonStartY + 2 * (sNumpadButtonH + sNumpadButtonSpacing), sNumpadButtonW, sNumpadButtonH, numpad7},
    // Numpad 8
    {sNumpadButtonStartX + sNumpadButtonW + sNumpadButtonSpacing, sNumpadButtonStartY + 2 * (sNumpadButtonH + sNumpadButtonSpacing), sNumpadButtonW, sNumpadButtonH, numpad8},
    // Numpad 9
    {sNumpadButtonStartX + 2 * (sNumpadButtonW + sNumpadButtonSpacing), sNumpadButtonStartY + 2 * (sNumpadButtonH + sNumpadButtonSpacing), sNumpadButtonW, sNumpadButtonH, numpad9},
    // Numpad C
    {sNumpadButtonStartX, sNumpadButtonStartY + 3 * (sNumpadButtonH + sNumpadButtonSpacing), sNumpadButtonW, sNumpadButtonH, numpadC},
    // Numpad 0
    {sNumpadButtonStartX + sNumpadButtonW + sNumpadButtonSpacing, sNumpadButtonStartY + 3 * (sNumpadButtonH + sNumpadButtonSpacing), sNumpadButtonW, sNumpadButtonH, numpad0},
    // Numpad OK
    {sNumpadButtonStartX + 2 * (sNumpadButtonW + sNumpadButtonSpacing), sNumpadButtonStartY + 3 * (sNumpadButtonH + sNumpadButtonSpacing), sNumpadButtonW, sNumpadButtonH, numpadOK}
};