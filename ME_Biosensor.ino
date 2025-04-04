// Send ME sensor data over indictaions to mobile device
// Displays results to 7in 800x480 touchscreen

#include <Adafruit_RA8875.h>
#include "ts_globalValues.h"
#include "ts_mainScreen_Helper_Functions.h"
#include "ts_buttonsCoordinates_Helper_Functions.h"

#include "ble_globalValues.h"
#include "ble_dataHandler.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.begin(115200);
  // Serial1.begin(115200);
  Serial.println("ME Biosensor BGM220P running");
  Serial.println("RA8875 start");

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, btn_state_change_callback, CHANGE);

  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1)
      ;
  }

  Serial.println("Found RA8875");
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  /* Enable the touch screen */
  Serial.println("Enabled the touch screen");
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
  tft.touchEnable(true);

  // With hardware accelleration this is instant
  tft.fillScreen(RA8875_WHITE);

  // Draw the initial GUI elements
  drawMainScreen();
}

void loop() {
  
  tsPoint_t rawTSpoint;
  tsPoint_t calibratedTSpoint;

  /* Clear the touch data object and placeholder variables */
  memset(&rawTSpoint, 0, sizeof(tsPoint_t));

  /* Clear any previous interrupts to avoid false buffered reads */
  uint16_t x, y;
  tft.touchRead(&x, &y);
  delay(10);

  /* Wait around for touch events */
  if (! digitalRead(RA8875_INT))
  {
    if (tft.touched())
    {
      tft.touchRead(&tx, &ty);
      rawTSpoint.x = tx;
      rawTSpoint.y = ty;

      calibrateTSPoint(&calibratedTSpoint, &rawTSpoint, &tsMatrix);

      // Check if on Graph/Main screen
      if (!isSettingsScreen){
        for (uint8_t i = 0; i < 8; i++) {
          if (isPointInsideButton(calibratedTSpoint.x, calibratedTSpoint.y, mainButtons[i])) {
            // Call the associated action function.
            mainButtons[i].action();
            break;  // Stop checking once a button is found.
          }
        }
      }
      else {
        for (uint8_t i = 0; i < 18; i++) {
          if (isPointInsideButton(calibratedTSpoint.x, calibratedTSpoint.y, settingsButtons[i])) {
            // Call the associated action function.
            settingsButtons[i].action();
            break;  // Stop checking once a button is found.
          }
        }
      }
      // Optional: Debounce or wait until touch is released
      delay(300);
    }
  }

  if (btn_state_changed) {
    btn_state_changed = false;
    if (numPoints != 0){
      clientReadyForNextIndication = true;
      handle_data_indication();
    }
  }

  if (ble_ringdown_parameters_changed) {
    // Parameters changed from BLE write
    // Update screen to reflect new values
    ble_ringdown_parameters_changed = false;
    drawMainScreen();

    sl_bt_gatt_server_write_attribute_value(ringdownParameters_characteristic_handle,
                                            0,
                                            sizeof(parametersRingdown), 
                                            (uint8_t *)&parametersRingdown);

    // Send indication back if paramaters were invalid and they were updated
    if (ringdownParameters_indication_enabled) {
      sl_bt_gatt_server_send_indication(connection_handle, 
                                        ringdownParameters_characteristic_handle, 
                                        sizeof(parametersRingdown), 
                                        (uint8_t *)&parametersRingdown);
    }
  }

  if (ts_ringdown_parameters_changed) {
    // Parameters changed from touch screen
    // Send indication to client to reflect new values
    ts_ringdown_parameters_changed = false;
    sl_bt_gatt_server_write_attribute_value(ringdownParameters_characteristic_handle,
                                            0,
                                            sizeof(parametersRingdown), 
                                            (uint8_t *)&parametersRingdown);

    if (ringdownParameters_indication_enabled) {
      sl_bt_gatt_server_send_indication(connection_handle, 
                                        ringdownParameters_characteristic_handle, 
                                        sizeof(parametersRingdown), 
                                        (uint8_t *)&parametersRingdown);
    }
  }
}
