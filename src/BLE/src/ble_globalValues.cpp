#include "ble_globalValues.h"

bool ringdownData_indication_enabled = false;
bool ringdownParameters_indication_enabled = false;
volatile bool btn_state_changed = false;
volatile uint8_t btn_state = LOW;

uint8_t connection_handle = 0u;
uint16_t ringdownData_characteristic_handle = 0u;
uint16_t ringdownParameters_characteristic_handle = 1u;

bool clientReadyForNextIndication = false;
uint32_t indicationIndex = 0;

bool ble_ringdown_parameters_changed = false;