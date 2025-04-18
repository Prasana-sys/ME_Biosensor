#include "ts_globalValues.h"
#include <string>
#include <EEPROM.h>
#include "ts_textCentering_Helper_Functions.h"
#include "ts_mainScreen_Helper_Functions.h"

void drawGraphAxes()
{
    // Draw Y axis
    tft.drawLine(graphX, graphY, graphX, graphY + graphH, RA8875_BLACK);

    // Draw X axis
    tft.drawLine(graphX, graphY + graphH, graphX + graphW, graphY + graphH, RA8875_BLACK);
}

void drawButtons()
{

    // Parameter buttons loop
    for (int i = 0; i < 5; i++)
    {
        // Draw the border for the button
        tft.drawRect(pButtonX[i], buttonY, pButtonW[i], buttonH, RA8875_BLACK);

        std::string paramBuffer = std::to_string(parametersRingdown[i]);
        if (i <= 2)
        {
            paramBuffer += " Hz";
        }
        else if (i == 3)
        {
            paramBuffer += " ms";
        }
        const char *lines[2] = {buttonLabels[i], paramBuffer.c_str()};

        drawCenteredMultiLine(tft, pButtonX[i], buttonY, pButtonW[i], buttonH, lines, 2);
    }

    // Control buttons loop
    for (int i = 0; i < 3; i++)
    {

        if (i == 0)
        {
            continue; // Skip drawing terminate box NOT IMPLEMENTED
            tft.fillRect(cButtonX[i], buttonY, cButtonW, buttonH, 0xfdb6);
        } // Terminate box red
        else if (i == 1)
        {
            tft.fillRect(cButtonX[i], buttonY, cButtonW, buttonH, 0xfff6);
        } // Reset box yellow
        else if (i == 2)
        {
            tft.fillRect(cButtonX[i], buttonY, cButtonW, buttonH, 0xc7f6);
        } // Start box green

        // Draw the border for the button
        tft.drawRect(cButtonX[i], buttonY, cButtonW, buttonH, RA8875_BLACK);

        drawCenteredText(tft, cButtonX[i], buttonY, cButtonW, buttonH, buttonLabels[5 + i]);
    }
}

void drawInfoBox()
{
    // Draw info rectangle
    tft.fillRect(infoX, infoY, infoW, infoH, RA8875_WHITE);
    tft.drawRect(infoX, infoY, infoW, infoH, RA8875_BLACK);

    // TODO: Need to fill with text
    tft.textMode();
    tft.textEnlarge(1);
    // tft.textSetCursor(textX, textY);
    // tft.textTransparent(RA8875_BLACK);
    // tft.textWrite("Graph");

    tft.graphicsMode();
}

void getXYMinMax(){

    // Error: No points found
    if (numPoints == 0) {
        _xyMinMax = {0, 0, 0, 0};
        Serial.println("Error: No points obtained from STM32");
    }

    ringdownData ringdownDataBuffer;

    EEPROM.get(0, ringdownDataBuffer);

    // Initialize min/max using the first data point.
    _xyMinMax.xMin = ringdownDataBuffer.frequency;
    _xyMinMax.xMax = ringdownDataBuffer.frequency;
    _xyMinMax.yMin = ringdownDataBuffer.duration;
    _xyMinMax.yMax = ringdownDataBuffer.duration;

    for (int eeAddress = sizeof(ringdownData); eeAddress < numPoints * sizeof(ringdownData); eeAddress += sizeof(ringdownData)) {
        EEPROM.get(eeAddress, ringdownDataBuffer);

        uint32_t freq = ringdownDataBuffer.frequency;
        uint8_t  dur  = ringdownDataBuffer.duration;
        
        if (freq < _xyMinMax.xMin) {
            _xyMinMax.xMin = freq;
        }
        if (freq > _xyMinMax.xMax) {
            _xyMinMax.xMax = freq;
        }
        if (dur < _xyMinMax.yMin) {
            _xyMinMax.yMin = dur;
        }
        if (dur > _xyMinMax.yMax) {
            _xyMinMax.yMax = dur;
        }
    }
}

void drawRingdownGraph()
{
    getXYMinMax();
    ringdownData ringdownDataBuffer, ringdownDataBuffer_1;

    Serial.println("Drawing Graph");

    // Draw the graph by mapping the data points to the graph area
    for (int eeAddress = 0; eeAddress < (numPoints * sizeof(ringdownData)) - sizeof(ringdownData); eeAddress += sizeof(ringdownData)) {

        EEPROM.get(eeAddress, ringdownDataBuffer);
        EEPROM.get(eeAddress + sizeof(ringdownData), ringdownDataBuffer_1);

        // Map x values to the graph width
        int x1 = graphX + map(ringdownDataBuffer.frequency, _xyMinMax.xMin, _xyMinMax.xMax, 0, graphW);
        int x2 = graphX + map(ringdownDataBuffer_1.frequency, _xyMinMax.xMin, _xyMinMax.xMax, 0, graphW);

        // Map y values (invert y-axis because screen y increases downward)
        int y1 = graphY + graphH - map(ringdownDataBuffer.duration, _xyMinMax.yMin, _xyMinMax.yMax, 0, graphH);
        int y2 = graphY + graphH - map(ringdownDataBuffer_1.duration, _xyMinMax.yMin, _xyMinMax.yMax, 0, graphH);

        // Draw line segments connecting the data points
        tft.drawLine(x1, y1, x2, y2, RA8875_BLACK);
    }

}

void drawMainScreen()
{
    // Clear or fill the entire screen first
    tft.fillScreen(RA8875_WHITE);

    drawGraphAxes();
    drawInfoBox();
    drawButtons();
}