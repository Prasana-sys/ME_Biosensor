#include <Arduino.h>

#ifndef BLE_INIT
#define BLE_INIT

/**************************************************************************/ /**
 * Initializes the GATT database
 * Creates a new GATT session and adds certain services and characteristics
 *****************************************************************************/
void ble_initialize_gatt_db();

/**************************************************************************/ /**
 * Starts BLE advertisement
 * Initializes advertising if it's called for the first time
 *****************************************************************************/
void ble_start_advertising();

#endif