#include <Arduino.h>

#ifndef BLE_GLOBAL_VALUES
#define BLE_GLOBAL_VALUES

extern bool ringdownData_indication_enabled;
extern bool ringdownParameters_indication_enabled;
extern volatile bool btn_state_changed;
extern volatile uint8_t btn_state;

const uint8_t advertised_name[] = "ME Biosensor";
extern uint8_t connection_handle;
extern uint8_t ringdownData_characteristic_handle;
extern uint8_t ringdownParameters_characteristic_handle;

extern bool clientReadyForNextIndication;
extern uint32_t indicationIndex;  // Number of steps

extern bool ble_ringdown_parameters_changed;

#pragma pack(push, 1)  // To condense the struct / remove extra byte padding
typedef struct {
  uint8_t duration;
  uint32_t frequency;
} ringdownData;
#pragma pack(pop)

#endif