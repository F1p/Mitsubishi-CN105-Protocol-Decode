/*
    Copyright (C) <2020>  <Mike Roberts>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Ecodan.h"

#include <ESPTelnet.h>
extern ESPTelnet TelnetServer;
#include "Debug.h"

// Initialisation Commands
uint8_t Init3[] = { 0xfc, 0x5a, 0x02, 0x7a, 0x02, 0xca, 0x01, 0x5d };  // Air to Water Connect


#define FIRST_READ_NUMBER_COMMANDS 37
uint8_t FirstReadActiveCommand[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x09, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                     0x10, 0x11, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                     0x1F, 0x20, 0x26, 0x27, 0x28, 0x29, 0xA1, 0xA2, 0x00 };

#define NUMBER_COMMANDS 68
uint8_t ActiveCommand[] = { 0x00, 0x01, 0x04, 0x02, 0x0C, 0x03, 0x04, 0x05, 0x0C, 0x06, 0x04, 0x07, 0x0C, 0x09, 0x04, 0x0B, 0x0C,
                            0x0D, 0x04, 0x0E, 0x0C, 0x0F, 0x04, 0x10, 0x0C, 0x11, 0x04, 0x13, 0x0C, 0x14, 0x04, 0x15, 0x0C, 0x16,
                            0x04, 0x17, 0x0C, 0x18, 0x04, 0x19, 0x0C, 0x1A, 0x04, 0x1B, 0x0C, 0x1C, 0x04, 0x1D, 0x0C, 0x1E, 0x04,
                            0x1F, 0x0C, 0x20, 0x04, 0x26, 0x0C, 0x27, 0x04, 0x28, 0x0C, 0x29, 0x04, 0xA1, 0x0C, 0xA2, 0x04, 0x00 };


#define NUMBER_SVC_COMMANDS 9
int ActiveServiceCode[] = { 3, 4, 5, 7, 10, 13, 19, 20, 22 };


unsigned long lastmsgdispatchedMillis = 0;  // variable for comparing millis counter
int cmd_queue_length = 0;
int cmd_queue_position = 1;
int CurrentWriteAttempt = 0;
bool WriteInProgress = false;

ECODAN::ECODAN(void)
  : ECODANDECODER() {
  CurrentMessage = 0;
  CurrentCommand = 0;
  CurrentSVCMessage = 0;
  UpdateFlag = 0;
  ProcessFlag = false;
  Connected = false;
  msbetweenmsg = 0;
}


void ECODAN::Process(void) {
  uint8_t c;

  while (DeviceStream->available()) {
    if (!ProcessFlag) {
      DEBUG_PRINT(F("[FTC > Bridge] "));
      ProcessFlag = true;
    }
    c = DeviceStream->read();

    if (c == 0)
      DEBUG_PRINT(F("__, "));
    else {
      if (c < 0x10) DEBUG_PRINT(F("0"));
      DEBUG_PRINT(String(c, HEX));
      DEBUG_PRINT(F(", "));
    }

    if (ECODANDECODER::Process(c)) {
      ProcessFlag = false;
      msbetweenmsg = millis() - lastmsgdispatchedMillis;
      DEBUG_PRINTLN();
      Connected = true;
    }
  }
}

void ECODAN::SetStream(Stream *HeatPumpStream) {
  DeviceStream = HeatPumpStream;
  Connect();
}


void ECODAN::TriggerSVCStateMachine(void){
  CurrentSVCMessage = 1;  // This triggers the running
}

void ECODAN::StopSVCStateMachine(void){
  CurrentSVCMessage = 0;  // This stops the running
}


void ECODAN::TriggerStatusStateMachine(void) {
  if (!Connected) {
    Connect();
  }
  CurrentMessage = 1;  // This triggers the run
  Connected = false;
}


void ECODAN::StopStateMachine(void) {
  if (CurrentMessage != 0) {
    DEBUG_PRINTLN(F("Stopping Heat Pump Read Operation to FTC"));
    CurrentMessage = 0;
  }
}


void ECODAN::StatusSVCMachine(void) {
  if (CurrentSVCMessage > 0) {
    WriteServiceCodeCMD(ActiveServiceCode[CurrentSVCMessage - 1]);
    CurrentSVCMessage++;
    CurrentSVCMessage %= NUMBER_SVC_COMMANDS;
    if (CurrentSVCMessage == 0) {
      CurrentSVCMessage = 1;
    }
  }
  else{
    CurrentSVCMessage = 1;
  }
}


void ECODAN::StatusStateMachine(void) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize;
  uint8_t i;

  if (CurrentMessage != 0) {
    DEBUG_PRINT(F("[Bridge > FTC] "));
    ECODANDECODER::CreateBlankTxMessage(GET_REQUEST, 0x10);

    if (Status.FTCVersion == 0) {
      ECODANDECODER::SetPayloadByte(FirstReadActiveCommand[CurrentMessage], 0);
    } else {
      ECODANDECODER::SetPayloadByte(ActiveCommand[CurrentMessage], 0);
    }

    CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
    DeviceStream->write(Buffer, CommandSize);
    lastmsgdispatchedMillis = millis();
    DeviceStream->flush();

    for (i = 0; i < CommandSize; i++) {
      if (Buffer[i] < 0x10) DEBUG_PRINT(F("0"));
      DEBUG_PRINT(String(Buffer[i], HEX));
      DEBUG_PRINT(F(", "));
    }
    DEBUG_PRINTLN();

    CurrentMessage++;

    if (Status.FTCVersion == 0) {
      CurrentMessage %= FIRST_READ_NUMBER_COMMANDS;  // Once none left
    } else {
      CurrentMessage %= NUMBER_COMMANDS;  // Once none left
    }

    // Straight to end
    if (CurrentMessage == 0) {
      UpdateFlag = 1;
    }
  }
}




void ECODAN::WriteStateMachine(void) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize;
  uint8_t i;

  if (cmd_queue_length > 0 && cmd_queue_length < 10) {
    CurrentWriteAttempt++;
    StopStateMachine();
    DEBUG_PRINT(F("Writing msg at position: "));
    DEBUG_PRINT(cmd_queue_position);
    DEBUG_PRINT(F(", attempt: "));
    DEBUG_PRINTLN(CurrentWriteAttempt);

    DEBUG_PRINT(F("[Bridge > FTC] "));
    ECODANDECODER::CreateBlankTxMessage(ECODANDECODER::ReturnNextCommandType(cmd_queue_position), 0x10);
    ECODANDECODER::EncodeNextCommand(cmd_queue_position);
    CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
    DeviceStream->write(Buffer, CommandSize);
    lastmsgdispatchedMillis = millis();
    DeviceStream->flush();

    for (i = 0; i < CommandSize; i++) {
      if (Buffer[i] < 0x10) DEBUG_PRINT(F("0"));
      DEBUG_PRINT(String(Buffer[i], HEX));
      DEBUG_PRINT(F(", "));
    }
    DEBUG_PRINTLN();

    WriteInProgress = true;
  }
}



void ECODAN::Connect(void) {
  DEBUG_PRINTLN(F("Connecting to Heat Pump..."));
  DeviceStream->write(Init3, 8);
  DeviceStream->flush();
  Process();
}

uint8_t ECODAN::HeatPumpConnected(void) {
  return Connected;
}

uint8_t ECODAN::UpdateComplete(void) {
  if (UpdateFlag) {
    UpdateFlag = 0;
    return 1;
  } else {
    return 0;
  }
}

uint8_t ECODAN::Lastmsbetweenmsg(void) {
  return msbetweenmsg;
}


void ECODAN::SetZoneTempSetpoint(float Setpoint, uint8_t Mode, uint8_t Zone) {
  ECODANDECODER::EncodeRoomThermostat(Setpoint, Mode, Zone);  // Can OR the write with the mode but removed as different MQTT topic:      SET_ZONE_SETPOINT | SET_HEATING_CONTROL_MODE
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::SetFlowSetpoint(float Setpoint, uint8_t Mode, uint8_t Zone) {
  ECODANDECODER::EncodeFlowTemperature(Setpoint, Mode, Zone);  // Can OR the write with the mode but removed as different MQTT topic:      SET_ZONE_SETPOINT | SET_HEATING_CONTROL_MODE
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::SetDHWMode(String *Mode) {
  if (*Mode == String("Normal")) {
    ECODANDECODER::EncodeDHWMode(0);
  } else if (*Mode == String("Eco")) {
    ECODANDECODER::EncodeDHWMode(1);
  }
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::ForceDHW(uint8_t OnOff) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  ECODANDECODER::EncodeForcedDHW(OnOff);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  cmd_queue_length++;
}


void ECODAN::SetHolidayMode(uint8_t OnOff) {
  ECODANDECODER::EncodeHolidayMode(OnOff);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  cmd_queue_length++;
}


void ECODAN::SetProhibits(uint8_t Flags, uint8_t OnOff) {
  ECODANDECODER::EncodeProhibit(Flags, OnOff);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::SetSvrControlMode(uint8_t OnOff, uint8_t DHW, uint8_t Z1H, uint8_t Z1C, uint8_t Z2H, uint8_t Z2C) {
  ECODANDECODER::EncodeServerControlMode(OnOff, DHW, Z1H, Z1C, Z2H, Z2C);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::SetHotWaterSetpoint(uint8_t Target) {
  ECODANDECODER::EncodeDHWSetpoint(Target);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::SetHeatingControlMode(uint8_t Mode, uint8_t Zone) {
  ECODANDECODER::EncodeControlMode(Mode, Zone);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::SetSystemPowerMode(uint8_t OnOff) {
  ECODANDECODER::EncodePower(OnOff);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::GetFTCVersion() {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  DEBUG_PRINT(F("[Bridge > FTC] "));
  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(0x5B, 0x01);
  ECODANDECODER::EncodeFTCVersion();
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT(F("0"));
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(F(", "));
  }
  DEBUG_PRINTLN();
}


void ECODAN::WriteMELCloudCMD(uint8_t cmd) {
  ECODANDECODER::EncodeMELCloud(cmd);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(SET_REQUEST, cmd_queue_length);
  DEBUG_PRINT(F("Transferred msg to position: "));
  DEBUG_PRINTLN(cmd_queue_length);
}


void ECODAN::WriteServiceCodeCMD(int cmd) {
  uint8_t UpperByte, LowerByte;

  ECODANDECODER::SetPayloadByte(0xa3, 0);
  UpperByte = (uint8_t)((cmd & 0xff00) >> 8);
  ECODANDECODER::SetPayloadByte(UpperByte, 1);
  LowerByte = (uint8_t)(cmd & 0x00ff);
  ECODANDECODER::SetPayloadByte(LowerByte, 2);
  cmd_queue_length++;
  ECODANDECODER::TransfertoBuffer(GET_REQUEST, cmd_queue_length);
}
