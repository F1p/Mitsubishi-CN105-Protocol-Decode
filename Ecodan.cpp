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

//[CONNECT_LEN] = {0xfc, 0x5a, 0x02, 0x7a, 0x02, 0xca, 0x01, 0x5d};

// Initialisation Commands

uint8_t Init1[] = { 0x02, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x02 };
uint8_t Init2[] = { 0x02, 0xff, 0xff, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00 };
uint8_t Init3[] = { 0xfc, 0x5a, 0x02, 0x7a, 0x02, 0xca, 0x01, 0x5d };
uint8_t Init4[] = { 0xfc, 0x5b, 0x02, 0x7a, 0x01, 0xc9, 0x5f };
uint8_t Init5[] = { 0xfc, 0x41, 0x02, 0x7a, 0x10, 0x34, 0x00, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0xfd };

#define NUMBER_COMMANDS 22
uint8_t ActiveCommand[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x09, 0x0B, 0x0C, 0x0D, 0x0E,
                            0x10, 0x13, 0x14, 0x15, 0x16,
                            0x26, 0x28, 0x29,
                            0xA1, 0xA2,
                            0x00 };


unsigned long lastmsgdispatchedMillis = 0;  // variable for comparing millis counter

ECODAN::ECODAN(void)
  : ECODANDECODER() {
  CurrentMessage = 0;
  UpdateFlag = 0;
  Connected = false;
  msbetweenmsg = 0;
}


void ECODAN::Process(void) {
  uint8_t c;

  while (DeviceStream->available()) {
    c = DeviceStream->read();

    if (c == 0)
      DEBUG_PRINT("__, ");
    else {
      if (c < 0x10) DEBUG_PRINT("0");
      DEBUG_PRINT(String(c, HEX));
      DEBUG_PRINT(", ");
    }

    if (ECODANDECODER::Process(c)) {
      msbetweenmsg = millis() - lastmsgdispatchedMillis;
      DEBUG_PRINTLN();
      DEBUG_PRINT(msbetweenmsg);
      DEBUG_PRINTLN("ms");
      Connected = true;
    }
  }
}

void ECODAN::SetStream(Stream *HeatPumpStream) {
  DeviceStream = HeatPumpStream;
  Connect();
}


void ECODAN::TriggerStatusStateMachine(void) {
  //DEBUG_PRINTLN("\e[1;1H\e[2J");  // Clear terminal
  DEBUG_PRINTLN("Triggering Heat Pump Query");
  if (!Connected) {
    Connect();
  }
  CurrentMessage = 1;  // This triggers the run
  Connected = false;
}


void ECODAN::StopStateMachine(void) {
  if (CurrentMessage != 0) {
    DEBUG_PRINTLN("Stopping Heat Pump Read Operation");
    CurrentMessage = 0;
    DeviceStream->flush();  // Clear the Serial Buffer
    int TimeSinceLastDispatch = millis() - lastmsgdispatchedMillis;
    if (TimeSinceLastDispatch > 0 && TimeSinceLastDispatch <= 500) {
      delay(500 - TimeSinceLastDispatch);  // Ensure a minimum spacing between msgs
    }
  }
}



void ECODAN::StatusStateMachine(void) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize;
  uint8_t i;

  if (CurrentMessage != 0) {
    DEBUG_PRINT("Send Message ");
    DEBUG_PRINTLN(CurrentMessage);
    ECODANDECODER::CreateBlankTxMessage(GET_REQUEST, 0x10);
    ECODANDECODER::SetPayloadByte(ActiveCommand[CurrentMessage], 0);
    CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
    DeviceStream->write(Buffer, CommandSize);
    lastmsgdispatchedMillis = millis();
    DeviceStream->flush();

    for (i = 0; i < CommandSize; i++) {
      if (Buffer[i] < 0x10) DEBUG_PRINT("0");
      DEBUG_PRINT(String(Buffer[i], HEX));
      DEBUG_PRINT(", ");
    }
    DEBUG_PRINTLN();

    CurrentMessage++;
    CurrentMessage %= NUMBER_COMMANDS;  // Once none left

    // Straight to end
    if (CurrentMessage == 0) {
      UpdateFlag = 1;
    }
  } else {
    PrintTumble();
  }
}


void ECODAN::RequestStatus(uint8_t TargetMessage) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize;
  uint8_t i;

  if (!Connected) { Connect(); }
  StopStateMachine();
  DEBUG_PRINT("Send Message ");
  DEBUG_PRINTLN(TargetMessage);
  ECODANDECODER::CreateBlankTxMessage(GET_REQUEST, 0x10);
  ECODANDECODER::SetPayloadByte(ActiveCommand[TargetMessage], 0);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::Connect(void) {
  DEBUG_PRINTLN("Connecting to Heat Pump...");
  DeviceStream->write(Init3, 8);
  DeviceStream->flush();
  Process();
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


void ECODAN::KeepAlive(void) {
  uint8_t CommandSize;
  uint8_t i;
  uint8_t Buffer[COMMANDSIZE];

  DEBUG_PRINTLN("Keep Alive Message...");
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::SetPayloadByte(0x34, 0);
  ECODANDECODER::SetPayloadByte(0x02, 1);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();

  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetZoneTempSetpoint(float Setpoint, uint8_t Mode, uint8_t Zone) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeRoomThermostat(Setpoint, Mode, Zone);  // Can OR the write with the mode but removed as different MQTT topic:      SET_ZONE_SETPOINT | SET_HEATING_CONTROL_MODE
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetFlowSetpoint(float Setpoint, uint8_t Mode, uint8_t Zone) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeFlowTemperature(Setpoint, Mode, Zone);  // Can OR the write with the mode but removed as different MQTT topic:      SET_ZONE_SETPOINT | SET_HEATING_CONTROL_MODE
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetDHWMode(String *Mode) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  if (*Mode == String("Normal")) {
    ECODANDECODER::EncodeDHWMode(0);
  } else if (*Mode == String("Eco")) {
    ECODANDECODER::EncodeDHWMode(1);
  }

  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::ForceDHW(uint8_t OnOff) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeDHW(OnOff);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetHolidayMode(uint8_t OnOff) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeHolidayMode(OnOff);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetProhibits(uint8_t Flags, uint8_t OnOff) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeProhibit(Flags, OnOff);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetSvrControlMode(uint8_t OnOff) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeServerControlMode(OnOff);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetHotWaterSetpoint(uint8_t Target) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeDHWSetpoint(Target);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetHeatingControlMode(uint8_t Mode) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodeControlMode(Mode);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::SetSystemPowerMode(uint8_t OnOff) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(SET_REQUEST, 0x10);
  ECODANDECODER::EncodePower(OnOff);
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);

  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::GetFTCVersion() {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize = 0;
  uint8_t i;

  StopStateMachine();
  ECODANDECODER::CreateBlankTxMessage(0x5B, 0x01);
  ECODANDECODER::EncodeFTCVersion();
  CommandSize = ECODANDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  lastmsgdispatchedMillis = millis();
  DeviceStream->flush();

  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void ECODAN::PrintTumble(void) {
  static char tumble[] = "|/-\\";
  static uint8_t i = 0;
  char c;

  DEBUG_PRINT('\b');
  c = tumble[i];
  DEBUG_PRINT(c);

  i++;
  i %= 4;
}
