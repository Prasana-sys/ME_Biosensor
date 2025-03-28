#include "ble_initialization.h"
#include "ble_globalValues.h"
#include "ts_globalValues.h"

/**************************************************************************/ /**
 * Initializes the GATT database
 * Creates a new GATT session and adds certain services and characteristics
 *****************************************************************************/
void ble_initialize_gatt_db() {
    sl_status_t sc;
    uint16_t gattdb_session_id;
    uint16_t generic_access_service_handle;
    uint16_t device_name_characteristic_handle;
    uint16_t ME_sensor_service_handle;
    // uint16_t data_characteristic_handle;
  
    // Create a new GATT database
    sc = sl_bt_gattdb_new_session(&gattdb_session_id);
    app_assert_status(sc);
  
    // Generic Access service
    const uint8_t generic_access_service_uuid[] = { 0x00, 0x18 };
    sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                  sl_bt_gattdb_primary_service,
                                  SL_BT_GATTDB_ADVERTISED_SERVICE,
                                  sizeof(generic_access_service_uuid),
                                  generic_access_service_uuid,
                                  &generic_access_service_handle);
    app_assert_status(sc);
  
    // Device Name characteristic
    const sl_bt_uuid_16_t device_name_characteristic_uuid = { .data = { 0x00, 0x2A } };
    sc = sl_bt_gattdb_add_uuid16_characteristic(gattdb_session_id,
                                                generic_access_service_handle,
                                                SL_BT_GATTDB_CHARACTERISTIC_READ,
                                                0x00,
                                                0x00,
                                                device_name_characteristic_uuid,
                                                sl_bt_gattdb_fixed_length_value,
                                                sizeof(advertised_name) - 1,
                                                sizeof(advertised_name) - 1,
                                                advertised_name,
                                                &device_name_characteristic_handle);
    app_assert_status(sc);
  
    sc = sl_bt_gattdb_start_service(gattdb_session_id, generic_access_service_handle);
    app_assert_status(sc);
  
    // ME Sensor service
    const uuid_128 ME_sensor_service_uuid = { .data = { 0x24, 0x12, 0xb5, 0xcb, 0xd4, 0x60, 0x80, 0x0c, 0x15, 0xc3, 0x9b, 0xa9, 0xac, 0x5a, 0x8a, 0xde } };
    sc = sl_bt_gattdb_add_service(gattdb_session_id,
                                  sl_bt_gattdb_primary_service,
                                  SL_BT_GATTDB_ADVERTISED_SERVICE,
                                  sizeof(ME_sensor_service_uuid),
                                  ME_sensor_service_uuid.data,
                                  &ME_sensor_service_handle);
    app_assert_status(sc);
  
    // ringdownData characteristic
    const uuid_128 ringdownData_characteristic_uuid = { .data = { 0x7a, 0x08, 0x6a, 0x73, 0x6c, 0xbe, 0xd8, 0x46, 0x97, 0xc2, 0x88, 0x40, 0x10, 0x65, 0x02, 0x5b } };
    // uint8_t data_initial_value[20] = {0};
  
    const ringdownData ringdownInitialValue = { .duration = 0, .frequency = 0 };
    // uint8_t *txBuffer = new uint8_t[sizeof(ringdownData)];
    // memcpy(txBuffer, &ringdownInitialValue, sizeof(ringdownData));
  
    sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                                 ME_sensor_service_handle,
                                                 SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_INDICATE,
                                                 0x00,
                                                 0x00,
                                                 ringdownData_characteristic_uuid,
                                                 sl_bt_gattdb_fixed_length_value,
                                                 5,
                                                 sizeof(ringdownInitialValue),
                                                 (uint8_t *)&ringdownInitialValue,
                                                 &ringdownData_characteristic_handle);
    app_assert_status(sc);

    // ringdownParameters characteristic cc143c32-089e-4204-b5a3-e9567c905b84
    const uuid_128 ringdownParameters_characteristic_uuid = { .data = { 0x84, 0x5b, 0x90, 0x7c, 0x56, 0xe9, 0xa3, 0xb5, 0x04, 0x42, 0x9e, 0x08, 0x32, 0x3c, 0x14, 0xcc } };
  
    // ringdownParameters defined in ts_globalValues.h

    sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                                 ME_sensor_service_handle,
                                                 SL_BT_GATTDB_CHARACTERISTIC_READ | SL_BT_GATTDB_CHARACTERISTIC_WRITE | SL_BT_GATTDB_CHARACTERISTIC_INDICATE,
                                                 0x00,
                                                 0x00,
                                                 ringdownParameters_characteristic_uuid,
                                                 sl_bt_gattdb_fixed_length_value,
                                                 20,
                                                 sizeof(parametersRingdown),
                                                 (uint8_t *)&parametersRingdown,
                                                 &ringdownParameters_characteristic_handle);
    app_assert_status(sc);

    // Start the ME sensor service
    sc = sl_bt_gattdb_start_service(gattdb_session_id, ME_sensor_service_handle);
    app_assert_status(sc);
  
    // Commit the GATT DB changes
    sc = sl_bt_gattdb_commit(gattdb_session_id);
    app_assert_status(sc);
}

/**************************************************************************/ /**
 * Starts BLE advertisement
 * Initializes advertising if it's called for the first time
 *****************************************************************************/
void ble_start_advertising() {
    static uint8_t advertising_set_handle = 0xff;
    static bool init = true;
    sl_status_t sc;
  
    if (init) {
      // Create an advertising set
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);
  
      // Set advertising interval to 100ms
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160,  // minimum advertisement interval (milliseconds * 1.6)
        160,  // maximum advertisement interval (milliseconds * 1.6)
        0,    // advertisement duration
        0);   // maximum number of advertisement events
      app_assert_status(sc);
  
      init = false;
    }
  
    // Generate data for advertising
    sc = sl_bt_legacy_advertiser_generate_data(advertising_set_handle, sl_bt_advertiser_general_discoverable);
    app_assert_status(sc);
  
    // Start advertising and enable connections
    sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_advertiser_connectable_scannable);
    app_assert_status(sc);
  
    Serial.print("Started advertising as '");
    Serial.print((const char *)advertised_name);
    Serial.println("'...");
}