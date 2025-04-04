#include "ble_dataHandler.h"

#include <Arduino.h>
#include <EEPROM.h>
#include "ble_globalValues.h"
#include "ts_globalValues.h"

void btn_state_change_callback() {
    // If the board has a built-in button
    #ifdef BTN_BUILTIN
      // The real button state is inverted - most boards have an active low button configuration
      btn_state = !digitalRead(BTN_BUILTIN);
      if (btn_state == 1)
        btn_state_changed = true;
    #endif  // BTN_BUILTIN
}
  
void handle_data_indication() {
    // Return immediately if indications are not enabled or client not ready for next indication
    if (!ringdownData_indication_enabled || !clientReadyForNextIndication) {
      return;
    }
  
    clientReadyForNextIndication = false;

    int eeAddress = indicationIndex * sizeof(ringdownData);
  
    // ringdownData newData = { .duration = indicationIndex + 1, .frequency = 400000 + (100 * indicationIndex) };
    // ringdownData newData = _ringdownData[indicationIndex];

    ringdownData newData;
    EEPROM.get(eeAddress, newData);
  
    // Send the indication
    sl_bt_gatt_server_send_indication(connection_handle, ringdownData_characteristic_handle, sizeof(newData), (uint8_t *)&newData);
}

void check_parameters_validity() {

  for (uint8_t i = 0; i < 5; i++){
    if (parametersRingdown[i] < parameterRanges[i].minValue) {
      parametersRingdown[i] = parameterRanges[i].minValue;
    }
    else if (parametersRingdown[i] > parameterRanges[i].maxValue) {
      parametersRingdown[i] = parameterRanges[i].maxValue;
    }
  }
}
