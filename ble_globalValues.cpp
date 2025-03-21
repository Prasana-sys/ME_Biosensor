#include "ble_globalValues.h"

bool indication_enabled = false;
volatile bool btn_state_changed = false;
volatile uint8_t btn_state = LOW;

uint8_t connection_handle = 0u;
uint16_t data_characteristic_handle = 0u;

bool clientReadyForNextIndication = false;
uint32_t indicationIndex = 0;