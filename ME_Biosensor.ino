// Send ME sensor data over indictaions to mobile device

static void ble_initialize_gatt_db();
static void ble_start_advertising();
static void btn_state_change_callback();
static void handle_data_indication();

bool indication_enabled = false;
volatile bool btn_state_changed = false;
volatile uint8_t btn_state = LOW;

const uint8_t advertised_name[] = "ME Biosensor";
uint8_t connection_handle = 0u;
uint16_t data_characteristic_handle = 0u;

#pragma pack(push, 1)
typedef struct {
  uint8_t duration;
  uint32_t frequency;
} ringdownData;
#pragma pack(pop)

uint8_t *txBuffer = new uint8_t[sizeof(ringdownData)];

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.begin(115200);
  Serial.println("ME Biosensor Arduino running");

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  attachInterrupt(BTN_BUILTIN, btn_state_change_callback, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (btn_state_changed) {
    btn_state_changed = false;
    handle_data_indication();
  }
}

static void btn_state_change_callback()
{
  // If the board has a built-in button
  #ifdef BTN_BUILTIN
  // The real button state is inverted - most boards have an active low button configuration
  btn_state = !digitalRead(BTN_BUILTIN);
  if (btn_state == 1)
    btn_state_changed = true;
  #endif // BTN_BUILTIN
}

/**************************************************************************//**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
    {
      Serial.println("BLE stack booted");

      // Initialize the application specific GATT table
      ble_initialize_gatt_db();

      // Start advertising
      ble_start_advertising();
      Serial.println("BLE advertisement started");
    }
    break;

    // -------------------------------
    // This event indicates that a new connection was opened
    case sl_bt_evt_connection_opened_id:
      Serial.println("BLE connection opened");
      // Store the connection handle which will be needed for sending indications
      connection_handle = evt->data.evt_connection_opened.connection;
      break;

    // -------------------------------
    // This event indicates that a connection was closed
    case sl_bt_evt_connection_closed_id:
      connection_handle = 0u;
      indication_enabled = false;
      Serial.println("BLE connection closed");
      delete[] txBuffer;
      // Restart the advertisement
      ble_start_advertising();
      Serial.println("BLE advertisement restarted");
      break;

    // -------------------------------
    // This event is received when a GATT characteristic status changes
    case sl_bt_evt_gatt_server_characteristic_status_id:
      // If the 'Data' characteristic has been changed
      if (evt->data.evt_gatt_server_characteristic_status.characteristic == data_characteristic_handle) {
        uint16_t client_config_flags = evt->data.evt_gatt_server_characteristic_status.client_config_flags;
        uint8_t status_flags = evt->data.evt_gatt_server_characteristic_status.status_flags;
        if ((client_config_flags == 0x02) && (status_flags == 0x01)) {
          // If indication was enabled (0x02) in the client config flags, and the status flag shows that it's a change
          Serial.println("Data indication enabled");
          indication_enabled = true;
        } else if ((client_config_flags == 0x00) && (status_flags == 0x01)) {
          // If indication was disabled (0x00) in the client config flags, and the status flag shows that it's a change
          Serial.println("Data indication disabled");
          indication_enabled = false;
        }
      }
      break;

    // -------------------------------
    // Default event handler
    default:
      break;
  }
}

static void handle_data_indication()
{
  // Return immediately if indications are not enabled
  if (!indication_enabled) {
    return;
  }

  // uint8_t data[20]; // Array to hold (x, y) pairs for 10 points
  // for (uint8_t i = 0; i < 10; i++) {
  //     data[i * 2] = i + 1;     // x values
  //     data[i * 2 + 1] = i + 1; // y values
  // }

  for (int i = 0; i < 10; i++) {
    ringdownData newData = {.duration = i+1, .frequency = 400000 + (100 * i)};
    // memcpy(txBuffer, &newData, sizeof(ringdownData));

    // Send the indication
    sl_bt_gatt_server_send_indication(connection_handle, data_characteristic_handle, sizeof(newData), (uint8_t *) &newData);

    delay(100);
  }

  // Wait for a second
  delay(1000);
}

/**************************************************************************//**
 * Starts BLE advertisement
 * Initializes advertising if it's called for the first time
 *****************************************************************************/
static void ble_start_advertising()
{
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
      160,   // minimum advertisement interval (milliseconds * 1.6)
      160,   // maximum advertisement interval (milliseconds * 1.6)
      0,     // advertisement duration
      0);    // maximum number of advertisement events
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
  Serial.print((const char*)advertised_name);
  Serial.println("'...");
}

/**************************************************************************//**
 * Initializes the GATT database
 * Creates a new GATT session and adds certain services and characteristics
 *****************************************************************************/
static void ble_initialize_gatt_db()
{
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

  // Data characteristic
  const uuid_128 data_characteristic_uuid = { .data = { 0x7a, 0x08, 0x6a, 0x73, 0x6c, 0xbe, 0xd8, 0x46, 0x97, 0xc2, 0x88, 0x40, 0x10, 0x65, 0x02, 0x5b } };
  // uint8_t data_initial_value[20] = {0};

  const ringdownData ringdownInitialValue = {.duration = 0, .frequency = 0};
  // uint8_t *txBuffer = new uint8_t[sizeof(ringdownData)];
  // memcpy(txBuffer, &ringdownInitialValue, sizeof(ringdownData));

  sc = sl_bt_gattdb_add_uuid128_characteristic(gattdb_session_id,
                                              ME_sensor_service_handle,
                                              SL_BT_GATTDB_CHARACTERISTIC_INDICATE,
                                              0x00,
                                              0x00,
                                              data_characteristic_uuid,
                                              sl_bt_gattdb_fixed_length_value,
                                              5,
                                              sizeof(ringdownInitialValue),
                                              (uint8_t *) &ringdownInitialValue,
                                              &data_characteristic_handle);
  app_assert_status(sc);

  // Start the Health Thermometer service
  sc = sl_bt_gattdb_start_service(gattdb_session_id, ME_sensor_service_handle);
  app_assert_status(sc);

  // Commit the GATT DB changes
  sc = sl_bt_gattdb_commit(gattdb_session_id);
  app_assert_status(sc);
}
