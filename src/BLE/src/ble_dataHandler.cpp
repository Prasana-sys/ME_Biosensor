#include "ble_dataHandler.h"

#include <Arduino.h>
#include "ble_globalValues.h"
#include "../../TouchScreen/src/ts_globalValues.h"

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

    // RINGDOWNDATA_START_ADDRESS[flashAddress] => duration
    // RINGDOWNDATA_START_ADDRESS[flashAddress + 1] => frequency
    int flashAddress = indicationIndex * 2;

    ringdownData newData;
    newData.duration = RINGDOWNDATA_START_ADDRESS[flashAddress];
    newData.frequency = RINGDOWNDATA_START_ADDRESS[flashAddress+1];
  
    // Send the indication
    sl_bt_gatt_server_send_indication(connection_handle, ringdownData_characteristic_handle, sizeof(newData), (uint8_t *)&newData);
}