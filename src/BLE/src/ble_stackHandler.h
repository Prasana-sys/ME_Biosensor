#include <Arduino.h>

#ifndef BLE_STACK_HANDLER
#define BLE_STACK_HANDLER

/**************************************************************************/ /**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt);

#endif