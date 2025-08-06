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

#ifndef ECODAN_h
#define ECODAN_h

#include <stdint.h>
#include <Arduino.h>
#include "EcodanDecoder.h"

class ECODAN : public ECODANDECODER {
public:
  ECODAN(void);
  void Process(void);
  void Disconnect(void);
  void SetStream(Stream *HeatPumpStream);
  void TriggerStatusStateMachine(void);
  void StopStateMachine(void);
  void StatusStateMachine(void);
  void StatusSVCMachine(void);
  void TriggerSVCStateMachine(void);
  void StopSVCStateMachine(void);
  void WriteStateMachine(void);
  uint8_t UpdateComplete(void);
  uint8_t SVCUpdateComplete(void);
  uint8_t HeatPumpConnected(void);
  uint8_t Lastmsbetweenmsg(void);

  void SetZoneTempSetpoint(float Setpoint, uint8_t Mode, uint8_t Zone);
  void SetFlowSetpoint(float Setpoint, uint8_t Mode, uint8_t Zone);
  void SetProhibits(uint8_t Flags, uint8_t OnOff);
  void ForceDHW(uint8_t OnOff);
  void SetDHWMode(String *Mode);
  void SetHolidayMode(uint8_t OnOff);
  void SetSvrControlMode(uint8_t OnOff, uint8_t DHW, uint8_t Z1H, uint8_t Z1C, uint8_t Z2H, uint8_t Z2C);
  void GetFTCVersion(void);
  void SetHotWaterSetpoint(uint8_t Target);
  void SetHeatingControlMode(uint8_t Mode, uint8_t Zone);
  void SetSystemPowerMode(uint8_t OnOff);
  void WriteMELCloudCMD(uint8_t cmd);
  void WriteServiceCodeCMD(int cmd);

  bool SVCPopulated;

protected:

private:
  uint8_t CurrentMessage, CurrentSVCMessage;
  uint8_t CurrentCommand;

  uint8_t UpdateFlag, SVCUpdateFlag;
  bool ProcessFlag;
  uint8_t Connected;

  uint8_t msbetweenmsg;

  MessageStruct TXMessage;
  Stream *DeviceStream;
  void Connect(void);
  void printCurrentTime(void);
};

#endif