#include "ts_settingsScreen_Helper_Functions.h"

#include <string>
#include <WatchdogTimer.h>
#include "ts_textCentering_Helper_Functions.h"
#include "ts_globalValues.h"

void drawSettingsButtons()
{

    // Parameter buttons loop
    for (uint8_t i = 0; i < 5; i++)
    {
        tft.fillRect(sParameterButtonX, sParameterButtonY[i], sParameterButtonW, sParameterButtonH, RA8875_WHITE);
        // Draw the border for the button
        tft.drawRect(sParameterButtonX, sParameterButtonY[i], sParameterButtonW, sParameterButtonH, RA8875_BLACK);

        std::string paramBuffer = std::to_string(parametersRingdown[i]);
        if (i <= 2)
        {
            paramBuffer += " Hz";
        }
        else if (i == 3)
        {
            paramBuffer += " ms";
        }

        std::string rangeText = std::string("Range: ") + std::string(parameterRanges[i].minValueText) + std::string("-") + 
                                std::string(parameterRanges[i].maxValueText);

        const char *lines[3] = {buttonLabels[i], paramBuffer.c_str(), rangeText.c_str()};

        drawCenteredMultiLine(tft, sParameterButtonX, sParameterButtonY[i], sParameterButtonW, sParameterButtonH, lines, sizeof(lines) / sizeof(lines[0]));
    }

    // Done button
    tft.drawRect(sDoneButtonX, sDoneButtonY, sDoneButtonW, sDoneButtonH, RA8875_BLACK);
    drawCenteredText(tft, sDoneButtonX, sDoneButtonY, sDoneButtonW, sDoneButtonH, "Done");
}

void drawSettingsNumpad()
{
    for (uint8_t row = 0; row < 4; row++)
    {
        for (uint8_t col = 0; col < 3; col++)
        {
            uint8_t index = row * 3 + col;
            int bx = sNumpadButtonStartX + col * (sNumpadButtonW + sNumpadButtonSpacing);
            int by = sNumpadButtonStartY + row * (sNumpadButtonH + sNumpadButtonSpacing);

            tft.drawRect(bx, by, sNumpadButtonW, sNumpadButtonH, RA8875_BLACK);
            drawCenteredText(tft, bx, by, sNumpadButtonW, sNumpadButtonH, keypadLabels[index], 1);
        }
    }
}

void drawSettingsScreen()
{
    // Clear or fill the entire screen first
    tft.fillScreen(RA8875_WHITE);

    // Title at the top
    tft.textMode();
    tft.textEnlarge(1);
    tft.textTransparent(RA8875_BLACK);
    tft.textSetCursor(336, 30);
    tft.textWrite("Settings");
    tft.graphicsMode();

    // Draw settings buttons
    drawSettingsButtons();

    // Draw settings numpad
    drawSettingsNumpad();
}

void drawChangingBox(changingParameter parameter, uint32_t newValue)
{
    tft.fillRect(310, 200, sParameterButtonW, sParameterButtonH, RA8875_WHITE);

    // Parameter should not be none, if it is reset device
    if (parameter == changingParameter::None)
    {
        Serial.println("Resetting Si Labs MCU and Screen becuase of invalid parameter.");
        WatchdogTimer.begin(WDOG_PERIOD_1_S);
    }
    tft.drawRect(310, 200, sParameterButtonW, sParameterButtonH, RA8875_BLACK);

    std::string paramBuffer = std::to_string(newValue);
    std::string buttonLabel = std::string(buttonLabels[static_cast<int>(parameter)]);
    if (buttonLabel == "Start Freq" || buttonLabel == "Stop Freq" || buttonLabel == "Step Size")
    {
        paramBuffer += " Hz";
    }
    else if (buttonLabel == "Duration")
    {
        paramBuffer += " ms";
    }
    const char *lines[2] = {buttonLabel.c_str(), paramBuffer.c_str()};

    drawCenteredMultiLine(tft, 310, 200, sParameterButtonW, sParameterButtonH, lines, 2);
}

void changeStartFreq()
{
    newValue = 0;
    isChangingValue = true;
    selectedParameter = changingParameter::Start_Freq;

    drawChangingBox(selectedParameter, newValue);
}

void changeStopFreq()
{
    newValue = 0;
    isChangingValue = true;
    selectedParameter = changingParameter::Stop_Freq;

    drawChangingBox(selectedParameter, newValue);
}

void changeStepSize()
{
    newValue = 0;
    isChangingValue = true;
    selectedParameter = changingParameter::Step_Size;

    drawChangingBox(selectedParameter, newValue);
}

void changeDuration()
{
    newValue = 0;
    isChangingValue = true;
    selectedParameter = changingParameter::Duration;

    drawChangingBox(selectedParameter, newValue);
}

void changeSweeps()
{
    newValue = 0;
    isChangingValue = true;
    selectedParameter = changingParameter::Sweeps;

    drawChangingBox(selectedParameter, newValue);
}