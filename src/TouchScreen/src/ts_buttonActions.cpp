#include "ts_buttonActions.h"

#include <WatchdogTimer.h>
#include <EEPROM.h>
#include <string>
#include "ts_settingsScreen_Helper_Functions.h"
#include "ts_mainScreen_Helper_Functions.h"
#include "ts_textCentering_Helper_Functions.h"

void doNothing () {

}

void startFreqMainButton() {
  Serial.println("Start Freq main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true; 
  changeStartFreq();
}

void stopFreqMainButton() {
  Serial.println("Start Freq main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeStopFreq(); 
}

void stepSizeMainButton() {
  Serial.println("Step Size main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeStepSize(); 
}

void durationMainButton() {
  Serial.println("Duration main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeDuration(); 
}

void sweepsMainButton() {
  Serial.println("Sweeps main button pressed");
  // Draw Settings Screen
  drawSettingsScreen(); 
  isSettingsScreen = true;
  changeSweeps(); 
}

// void terminateMainButton() {
//   Serial.println("Terminate main button pressed");
//   Serial.println("Terminating ringdown process");

//   // TODO: Need to handle termination
//   digitalWrite(STM_RES, LOW);
//   delay(100);
//   digitalWrite(STM_RES, HIGH);
// }

void resetMainButton() {
  Serial.println("Resetting whole system...");
  WatchdogTimer.begin(WDOG_PERIOD_257_MS);
}

void sendParametersPacket() {
  Serial.println("Sending Parameters");

  #pragma pack(push, 1)  // To condense the struct / remove extra byte padding
  typedef struct {
    uint8_t StartOfFrame;
    uint32_t startFrequency;
    uint32_t stopFrequency;
    uint32_t stepSize;
    uint32_t frDuration;
    uint32_t sweeps;
    uint8_t EndOfFrame;
    uint8_t checksum;
  } parameterPacket;
  #pragma pack(pop)

  uint8_t checksum = (uint8_t)((parametersRingdown[0]+parametersRingdown[1]+parametersRingdown[2]+parametersRingdown[3]+parametersRingdown[4]) & 0xFF);

  parameterPacket packet = {0xA5, parametersRingdown[0], parametersRingdown[1], parametersRingdown[2],
                            parametersRingdown[3], parametersRingdown[4], 0x5A, checksum};

  Serial.print("Sending: ");
  Serial.write((byte *)&packet, sizeof(packet));
  Serial.println();
  Serial1.flush();
  Serial1.write((uint8_t *)&packet, sizeof(packet));
}

void receiveDataPackets() {

  uint8_t index = 0;
  int eeAddress = 0; //EEPROM address to start writing from
  int expectedNumberOfPoints= ((parametersRingdown[1] - parametersRingdown[0])/parametersRingdown[2]) + 1;

  #pragma pack(push, 1)  // To condense the struct / remove extra byte padding
  typedef struct {
    uint8_t StartOfFrame;
    uint8_t duration;
    uint32_t frequency;
    uint8_t EndOfFrame;
    uint8_t checksum;
  } ringdownPacket;
  #pragma pack(pop)

  uint8_t buffer[sizeof(ringdownPacket)];

  // Begin Watchdog Timer to monitor if getting data packets correctly
  WatchdogTimer.begin(WDOG_PERIOD_4_S);

  drawInfoBox();
  drawCenteredText(tft, infoX, infoY, infoW, infoH, "Running...");

  while (numPoints < expectedNumberOfPoints) {
    // Serial.println("Inside while loop");
    if (Serial1.available()){
      // Serial.println("Serial1 available");
      uint8_t inByte = (uint8_t)Serial1.read();
      // Serial.print("Incoming: ");
      // Serial.println(inByte, HEX);
      // If we haven't started buffering yet, look for the start-of-frame marker.
      if (index == 0) {
        if (inByte == 0xAA) {
          buffer[index++] = inByte;
        }
      }
      else {
        buffer[index++] = inByte;

        // Once we have received the full packet, process it.
        if (index == sizeof(ringdownPacket)) {
          // Cast buffer to our packet structure for easier access.
          ringdownPacket *packet = (ringdownPacket *)buffer;
          // Serial.print("Start Frame: ");
          // Serial.println(packet->StartOfFrame, HEX);
          // Serial.print("End Frame: ");
          // Serial.println(packet->EndOfFrame, HEX);
          // Serial.print("Duration: ");
          // Serial.println(packet->duration, HEX);
          // Serial.print("Frequency: ");
          // Serial.println(packet->frequency, HEX);
          // Serial.print("Checksum: ");
          // Serial.println(packet->checksum, HEX);

          // Validate end-of-frame marker.
          if (packet->EndOfFrame == 0xAA) {
            // // Calculate checksum over all bytes except the checksum field.
            // uint8_t calcChecksum = 0;
            // for (int i = 0; i < sizeof(ringdownPacket) - 1; i++) {
            //   calcChecksum += buffer[i];
            // }

            // if (calcChecksum == packet->checksum) {
              // Valid packet received; process the data.
              
              ringdownData dataBuffer = {packet->duration, packet->frequency};
              // Serial.print("Received packet: ");
              // Serial.print(dataBuffer.duration);
              // Serial.print(", ");
              // Serial.println(dataBuffer.frequency);

              drawInfoBox();
              std::string durationBuffer = "Duration: " + std::to_string(dataBuffer.duration);
              std::string frequencyBuffer = "Frequency: " + std::to_string(dataBuffer.frequency);
              const char *lines[3] = {"Received Ringdown packet:", durationBuffer.c_str(), frequencyBuffer.c_str()};
              drawCenteredMultiLine(tft, infoX, infoY, infoW, infoH, lines, 3);

              EEPROM.put(eeAddress, dataBuffer);

              eeAddress += sizeof(ringdownData);
              numPoints++;
              WatchdogTimer.feed(); // Feed timer after receiving a valid packet
              Serial1.flush();
            // }
            // else {
              // Serial.println("Checksum error");
            // }
          }
          else {
            Serial.println("End-of-frame marker error");
          }

          index = 0;
        }
      }
    }
    else {
      // Serial.println("Serial1 not available");
    }
  }

  WatchdogTimer.end();

  drawMainScreen();
  drawCenteredText(tft, infoX, infoY, infoW, infoH, "Finished!");
}

void startMainButton() {
  Serial.println("Start button pressed");

  // Send UART communication to STM to start ringdown and receive data back
  // Store UART packets in EEPROM
  // Increment numPoints based on incoming data

  numPoints = 0; // Reset numPoints to take in new data

  if (!EXPO_BACKUP_DEMO_MODE) {
    // If not in backup mode perform function as normal
    sendParametersPacket();

    Serial.println("Beginning to wait for packets");

    receiveDataPackets();
  }
  else {
    int eeAddress = 0;
    uint8_t durations[101] = {
      65, 67, 63, 65, 64, 64, 64, 65, 64, 63, 
      64, 63, 64, 67, 64, 66, 67, 69, 74, 78, 
      85, 95, 113, 120, 128, 138, 153, 152, 190, 184, 
      172, 206, 175, 215, 212, 198, 215, 188, 184, 181, 
      171, 139, 143, 114, 122, 107, 101, 75, 89, 70, 
      65, 82, 63, 63, 66, 64, 66, 66, 66, 62, 
      65, 63, 65, 65, 65, 63, 61, 64, 62, 63, 
      64, 65, 63, 63, 62, 63, 63, 62, 64, 62, 
      63, 61, 60, 63, 62, 62, 63, 64, 61, 61, 
      62, 63, 62, 63, 62, 62, 64, 64, 65, 62, 
      66
    };

    drawInfoBox();
    drawCenteredText(tft, infoX, infoY, infoW, infoH, "Running...");

    for (int i = 0; i <= 100; i++) {
      uint32_t frequency = 440000 + i * 100;

      ringdownData dataBuffer = {durations[i], frequency};

      EEPROM.put(eeAddress, dataBuffer);

      eeAddress += sizeof(ringdownData);
      numPoints++;

      Serial.print("Loop: ");
      Serial.println(i);

      delay (100);
    }

    drawMainScreen();
    drawCenteredText(tft, infoX, infoY, infoW, infoH, "Finished!");
  }

  // // For testing Only
  // numPoints = 10;

  // Draw the ringdown graph using the provided data
  drawRingdownGraph();

  Serial.println("Leaving Start button logic");
}

void startFreqSettingsButton() {
  Serial.println("Start Freq settings button pressed");
  changeStartFreq();
}

void stopFreqSettingsButton() {
  Serial.println("Stop Freq settings button pressed");
  changeStopFreq();
}

void stepSizeSettingsButton() {
  Serial.println("Step size settings button pressed");
  changeStepSize();
}

void durationSettingsButton() {
  Serial.println("Duration settings button pressed");
  changeDuration();
}

void sweepsSettingButton() {
  Serial.println("Sweep settings button pressed");
  changeSweeps();
}

void doneSettingsButton() {
  Serial.println("Done button pressed");
  // Draw Graph/Main Screen
  drawMainScreen();
  isSettingsScreen = false;
  isChangingValue = false;
  newValue = 0;
  selectedParameter = changingParameter::None;
}

void numpad1() {
  Serial.println("1 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 1;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad2() {
  Serial.println("2 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 2;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad3() {
  Serial.println("3 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 3;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad4() {
  Serial.println("4 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 4;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad5() {
  Serial.println("5 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 5;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad6() {
  Serial.println("6 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 6;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad7() {
  Serial.println("7 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 7;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad8() {
  Serial.println("8 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 8;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad9() {
  Serial.println("9 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 9;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpadC() {
  Serial.println("C button pressed");
  if (isChangingValue){
    newValue = 0;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpad0() {
  Serial.println("0 button pressed");
  if (isChangingValue){
    newValue = newValue * 10 + 0;
    drawChangingBox(selectedParameter, newValue);
  }
}

void numpadOK() {
  Serial.println("OK button pressed");
  if (isChangingValue){

    // Check if newValue is in valid range of selectedParameter
    if (newValue < parameterRanges[static_cast<int>(selectedParameter)].minValue) {
      newValue = parameterRanges[static_cast<int>(selectedParameter)].minValue;
    }
    else if (newValue > parameterRanges[static_cast<int>(selectedParameter)].maxValue) {
      newValue = parameterRanges[static_cast<int>(selectedParameter)].maxValue;
    }
    
    parametersRingdown[static_cast<int>(selectedParameter)] = newValue;

    check_parameters_validity(true);

    // Clear out changing box
    tft.fillRect(310, 200, sParameterButtonW, sParameterButtonH, RA8875_WHITE);

    ts_ringdown_parameters_changed = true;
    isChangingValue = false;
    selectedParameter = changingParameter::None;
    newValue = 0;

    // Redraw Settings buttons
    drawSettingsButtons();
  }
}