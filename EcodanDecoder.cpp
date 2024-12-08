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
#include "EcodanDecoder.h"
#include <cstdio>


ECODANDECODER::ECODANDECODER(void) {
  memset(&RxMessage, 0, sizeof(MessageStruct));
  memset(&Status, 0, sizeof(EcodanStatus));

  Preamble[0] = 0x02;
  Preamble[1] = 0x7a;
}


uint8_t ECODANDECODER::Process(uint8_t c) {
  uint8_t ReturnValue = false;

  if (BuildRxMessage(&RxMessage, c)) {
    ReturnValue = true;
    if (RxMessage.PacketType == GET_RESPONSE) {
      switch (RxMessage.Payload[0]) {
        case 0x01:
          Process0x01(RxMessage.Payload, &Status);
          break;
        case 0x02:
          Process0x02(RxMessage.Payload, &Status);
          break;
        case 0x03:
          Process0x03(RxMessage.Payload, &Status);
          break;
        case 0x04:
          Process0x04(RxMessage.Payload, &Status);
          break;
        case 0x05:
          Process0x05(RxMessage.Payload, &Status);
          break;
        case 0x07:
          Process0x07(RxMessage.Payload, &Status);
          break;
        case 0x09:
          Process0x09(RxMessage.Payload, &Status);
          break;
        case 0x0b:
          Process0x0B(RxMessage.Payload, &Status);
          break;
        case 0x0c:
          Process0x0C(RxMessage.Payload, &Status);
          break;
        case 0x0d:
          Process0x0D(RxMessage.Payload, &Status);
          break;
        case 0x0e:
          Process0x0E(RxMessage.Payload, &Status);
          break;
        case 0x0f:
          Process0x0F(RxMessage.Payload, &Status);
          break;
        case 0x10:
          Process0x10(RxMessage.Payload, &Status);
          break;
        case 0x11:
          Process0x11(RxMessage.Payload, &Status);
          break;
        case 0x13:
          Process0x13(RxMessage.Payload, &Status);
          break;
        case 0x14:
          Process0x14(RxMessage.Payload, &Status);
          break;
        case 0x15:
          Process0x15(RxMessage.Payload, &Status);
          break;
        case 0x16:
          Process0x16(RxMessage.Payload, &Status);
          break;
        case 0x26:
          Process0x26(RxMessage.Payload, &Status);
          break;
        case 0x28:
          Process0x28(RxMessage.Payload, &Status);
          break;
        case 0x29:
          Process0x29(RxMessage.Payload, &Status);
          break;
        case 0xa1:
          Process0xA1(RxMessage.Payload, &Status);
          break;
        case 0xa2:
          Process0xA2(RxMessage.Payload, &Status);
          break;
      }
    } else if (RxMessage.PacketType == GET_ABOUT_RESPONSE) {
      switch (RxMessage.Payload[0]) {
        case 0xc9:
          Process0xC9(RxMessage.Payload, &Status);
          break;
      }
    } else if (RxMessage.PacketType == SET_RESPONSE) {
      WriteOK(RxMessage.Payload, &Status);
    }
  }
  return ReturnValue;
}

uint8_t ECODANDECODER::BuildRxMessage(MessageStruct *Message, uint8_t c) {
  static uint8_t Buffer[COMMANDSIZE];
  static uint8_t BufferPos = 0;
  static uint8_t PayloadSize = 0;
  uint8_t i;

  if (BufferPos < HEADERSIZE) {
    switch (BufferPos) {
      case 0:
        if (c != PACKET_SYNC) return false;
        break;

      case 1:
        switch (c) {
          case SET_REQUEST:
            break;
          case SET_RESPONSE:
            break;
          case GET_REQUEST:
            break;
          case GET_RESPONSE:
            break;
          case CONNECT_REQUEST:
            break;
          case CONNECT_RESPONSE:
            break;
          case EXCONNECT_REQUEST:
            break;
          case EXCONNECT_RESPONSE:
            break;
          default:
            //Serial.println("Unknown PacketType");
            BufferPos = 0;
            return false;  // Unknown Packet Type
        }
        break;

      case 2:
        if (c != Preamble[0]) {
          //Serial.println("Preamble 1 Error");
          BufferPos = 0;
          return false;
        }
        break;

      case 3:
        if (c != Preamble[1]) {
          //Serial.println("Preamble 1 Error");
          BufferPos = 0;
          return false;
        }
        break;

      case 4:
        PayloadSize = c;
        if (c > MAXDATABLOCKSIZE) {
          //Serial.println("Oversize Payload");
          BufferPos = 0;
          return false;
        }
        break;
    }

    Buffer[BufferPos] = c;
    BufferPos++;
    return false;
  } else if (BufferPos < (PayloadSize + HEADERSIZE)) {
    Buffer[BufferPos] = c;
    BufferPos++;
  }

  else if (BufferPos == (PayloadSize + HEADERSIZE)) {
    Buffer[BufferPos] = c;
    BufferPos = 0;
    if (CheckSum(Buffer, PayloadSize + HEADERSIZE) == c) {
      //Serial.println("CS OK");
      Message->SyncByte = Buffer[0];
      Message->PacketType = Buffer[1];
      Message->Preamble[0] = Buffer[2];
      Message->Preamble[1] = Buffer[3];
      Message->PayloadSize = Buffer[4];
      Message->Checksum = c;
      memcpy(Message->Payload, &Buffer[5], Message->PayloadSize);
      return true;
    } else {
      //Serial.println("Checksum Fail");
      return false;
    }
  }
  return false;
}


void ECODANDECODER::Process0x01(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t Year, Month, Day;
  uint8_t Hour, Min, Sec;

  Year = Buffer[1];
  Month = Buffer[2];
  Day = Buffer[3];
  Hour = Buffer[4];
  Min = Buffer[5];
  Sec = Buffer[6];

  Status->DateTimeStamp.tm_year = 100 + Year;
  Status->DateTimeStamp.tm_mon = Month - 1;
  Status->DateTimeStamp.tm_mday = Day;

  Status->DateTimeStamp.tm_hour = Hour;
  Status->DateTimeStamp.tm_min = Min;
  Status->DateTimeStamp.tm_sec = Sec;

  snprintf(Status->FTCSoftware, 6, "%02X.%02X", Buffer[7], Buffer[8]);
}

void ECODANDECODER::Process0x02(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t Defrost;

  //Unknown = Buffer[1];  // Value 01
  //Unknown = Buffer[2];  // Value 02
  Defrost = Buffer[3];

  Status->Defrost = Defrost;
}

void ECODANDECODER::Process0x03(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t RefrigeFltCode, ErrCode1, ErrCode2, FltCode1, FltCode2;
  uint8_t TwoZone_Z1Working, TwoZone_Z2Working;

  RefrigeFltCode = Buffer[1];
  ErrCode1 = Buffer[2];
  ErrCode2 = Buffer[3];
  FltCode1 = Buffer[4];
  FltCode2 = Buffer[5];

  if (Buffer[8] == 1) {
    TwoZone_Z1Working = 1;
    TwoZone_Z2Working = 1;
  } else if (Buffer[8] == 2) {
    TwoZone_Z1Working = 1;
    TwoZone_Z2Working = 0;
  } else if (Buffer[8] == 3) {
    TwoZone_Z1Working = 0;
    TwoZone_Z2Working = 1;
  } else {
    TwoZone_Z1Working = 0;
    TwoZone_Z2Working = 0;
  }

  //CascadeMasterFreq = Buffer[9];  // Single Zone Running Parameter?

  Status->RefrigeFltCode = RefrigeFltCode;
  Status->ErrCode1 = ErrCode1;
  Status->ErrCode2 = ErrCode2;
  Status->FltCode1 = FltCode1;
  Status->FltCode2 = FltCode2;
  Status->TwoZone_Z1Working = TwoZone_Z1Working;
  Status->TwoZone_Z2Working = TwoZone_Z2Working;
}


void ECODANDECODER::Process0x04(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t CompressorFrequency;

  CompressorFrequency = Buffer[1];

  Status->CompressorFrequency = CompressorFrequency;
}


void ECODANDECODER::Process0x05(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t HotWaterBoost, DHWActive;
  uint8_t DHWHeatSourcePhase;

  //if (Buffer[5] == 7)         // Eco Hot Water Running
  //HeatSource = Buffer[6];     // 0 = H/P, 1 = IH, 2 = BH, 3 = IH + BH, 4 = Boiler but doesn't seem to change
  DHWHeatSourcePhase = Buffer[7];  // Heat Source Phase for DHW (0 = Normal, 1 = HP, 2 = Immersion or Booster)
  if (DHWHeatSourcePhase != 0) {
    DHWActive = 1;
  } else {
    DHWActive = 0;
  }
  //Unknown = Buffer[9];        // Always 6?

  //Status->HeatSource = HeatSource;
  Status->DHWHeatSourcePhase = DHWHeatSourcePhase;
  Status->DHWActive = DHWActive;
}


void ECODANDECODER::Process0x07(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t InputPower, OutputPower;

  InputPower = Buffer[4];
  OutputPower = Buffer[6];

  Status->InputPower = InputPower;
  Status->OutputPower = OutputPower;
}


void ECODANDECODER::Process0x09(uint8_t *Buffer, EcodanStatus *Status) {
  float Zone1TempSetpoint, Zone2TempSetpoint;
  float Z1FlowSetpoint, Z2FlowSetpoint, LegionellaSetpoint;
  float HWTempDrop, FlowTempMax, FlowTempMin;

  Zone1TempSetpoint = ((float)ExtractUInt16(Buffer, 1) / 100);
  Zone2TempSetpoint = ((float)ExtractUInt16(Buffer, 3) / 100);
  Z1FlowSetpoint = ((float)ExtractUInt16(Buffer, 5) / 100);
  Z2FlowSetpoint = ((float)ExtractUInt16(Buffer, 7) / 100);
  LegionellaSetpoint = ((float)ExtractUInt16(Buffer, 9) / 100);

  HWTempDrop = ExtractUInt8_v2(Buffer, 11);
  FlowTempMax = ExtractUInt8_v2(Buffer, 12);
  FlowTempMin = ExtractUInt8_v2(Buffer, 13);

  Status->Zone1TemperatureSetpoint = Zone1TempSetpoint;
  Status->Zone2TemperatureSetpoint = Zone2TempSetpoint;
  Status->Zone1FlowTemperatureSetpoint = Z1FlowSetpoint;
  Status->Zone2FlowTemperatureSetpoint = Z2FlowSetpoint;
  Status->LegionellaSetpoint = LegionellaSetpoint;
  Status->HotWaterMaximumTempDrop = HWTempDrop;
  Status->FlowTempMax = FlowTempMax;
  Status->FlowTempMin = FlowTempMin;
}

void ECODANDECODER::Process0x0B(uint8_t *Buffer, EcodanStatus *Status) {
  float fZone1, fZone2, fOutside;
  float RefrigeTemp;

  fZone1 = ((float)ExtractUInt16(Buffer, 1) / 100);
  if (Buffer[3] != 0xf0) {  // Extract if zone connected (not default value)
    fZone2 = ((float)ExtractUInt16(Buffer, 3) / 100);
  } else {
    fZone2 = 0;
  }

  //Unknown = ((float)ExtractUInt16(Buffer, 5) / 100);
  RefrigeTemp = ((float)ExtractUInt16_Signed(Buffer, 8) / 100);
  //Unknown = ExtractUInt8_v1(Buffer, 10);
  fOutside = ExtractUInt8_v1(Buffer, 11);

  Status->Zone1Temperature = fZone1;
  Status->Zone2Temperature = fZone2;
  Status->OutsideTemperature = fOutside;  // TH7
  Status->RefrigeTemp = RefrigeTemp;      //
}

void ECODANDECODER::Process0x0C(uint8_t *Buffer, EcodanStatus *Status) {
  float fWaterHeatingFeed, fWaterHeatingReturn, fHotWater, fHotWaterTHW5A;

  fWaterHeatingFeed = ((float)ExtractUInt16(Buffer, 1) / 100);
  fWaterHeatingReturn = ((float)ExtractUInt16(Buffer, 4) / 100);
  fHotWater = ((float)ExtractUInt16(Buffer, 7) / 100);
  fHotWaterTHW5A = ((float)ExtractUInt16(Buffer, 10) / 100);

  Status->HeaterOutputFlowTemperature = fWaterHeatingFeed;         // THW1
  Status->HeaterReturnFlowTemperature = fWaterHeatingReturn;       // THW2
  Status->HeaterDeltaT = fWaterHeatingFeed - fWaterHeatingReturn;  // Flow - Return Temp
  Status->HotWaterTemperature = fHotWater;                         // THW5
  Status->HotWaterTemperatureTHW5A = fHotWaterTHW5A;               // THW5A
}

void ECODANDECODER::Process0x0D(uint8_t *Buffer, EcodanStatus *Status) {
  float Zone1FlowTemperature, Zone1ReturnTemperature, Zone2FlowTemperature, Zone2ReturnTemperature;

  Zone1FlowTemperature = ((float)ExtractUInt16(Buffer, 1) / 100);     // Zone 1 Flow (THW6)
  Zone1ReturnTemperature = ((float)ExtractUInt16(Buffer, 4) / 100);   // Zone 1 Return (THW7)
  Zone2FlowTemperature = ((float)ExtractUInt16(Buffer, 7) / 100);     // Zone 2 Flow (THW8)
  Zone2ReturnTemperature = ((float)ExtractUInt16(Buffer, 10) / 100);  // Zone 2 Return (THW9)

  Status->Zone1FlowTemperature = Zone1FlowTemperature;
  Status->Zone1ReturnTemperature = Zone1ReturnTemperature;
  Status->Zone2FlowTemperature = Zone2FlowTemperature;
  Status->Zone2ReturnTemperature = Zone2ReturnTemperature;
}

void ECODANDECODER::Process0x0E(uint8_t *Buffer, EcodanStatus *Status) {
  float ExternalBoilerFlowTemperature, ExternalBoilerReturnTemperature;

  ExternalBoilerFlowTemperature = ((float)ExtractUInt16(Buffer, 1) / 100);    // Suspected (THWB1)
  ExternalBoilerReturnTemperature = ((float)ExtractUInt16(Buffer, 4) / 100);  // Suspected (THWB2)
  //Unknown = ((float)ExtractUInt16(Buffer, 10) / 100);                       // Unknown

  Status->ExternalBoilerFlowTemperature = ExternalBoilerFlowTemperature;
  Status->ExternalBoilerReturnTemperature = ExternalBoilerReturnTemperature;
}


void ECODANDECODER::Process0x0F(uint8_t *Buffer, EcodanStatus *Status) {  // FTC6 Only Parameters
  float MixingTemperature, CondensingTemp;

  MixingTemperature = ((float)ExtractUInt16(Buffer, 1) / 100);  // Mixing Tank Temperature (THW10)
  CondensingTemp = ((float)ExtractUInt16(Buffer, 4) / 100);     // Condensing Temperature

  Status->MixingTemperature = MixingTemperature;
  Status->CondensingTemp = CondensingTemp;
}


void ECODANDECODER::Process0x10(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t Zone1ThermostatDemand, Zone2ThermostatDemand, OutdoorThermostatDemand;

  Zone1ThermostatDemand = Buffer[1];
  Zone2ThermostatDemand = Buffer[2];
  OutdoorThermostatDemand = Buffer[3];

  Status->Zone1ThermostatDemand = Zone1ThermostatDemand;
  Status->Zone2ThermostatDemand = Zone2ThermostatDemand;
  Status->OutdoorThermostatDemand = OutdoorThermostatDemand;
}


void ECODANDECODER::Process0x11(uint8_t *Buffer, EcodanStatus *Status) {

  //Unknown2 = Buffer[1];  // 206 or 0, unknown what it is
  //Unknown3 = Buffer[3];  // 128 or 1 when unknown 2 is 0
  //Unknown4 = Buffer[5];  // 112 or 0 when unknown 2 is 0
  //Unknown5 = Buffer[9];  // 2 or 0 when unknown 2 is 0
}


void ECODANDECODER::Process0x13(uint8_t *Buffer, EcodanStatus *Status) {
  uint32_t RunHours;

  //Unknown6 = Buffer[1];  // 0 When off, on/off 1's when running

  RunHours = Buffer[4];
  RunHours = RunHours << 8;
  RunHours += Buffer[5];
  RunHours *= 100;
  RunHours += Buffer[3];

  Status->RunHours = RunHours;
}


void ECODANDECODER::Process0x14(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t FlowRate;
  uint8_t BoosterActive, ImmersionActive;

  BoosterActive = Buffer[2];
  ImmersionActive = Buffer[5];
  FlowRate = Buffer[12];

  Status->BoosterActive = BoosterActive;
  Status->ImmersionActive = ImmersionActive;
  Status->PrimaryFlowRate = FlowRate;
}


void ECODANDECODER::Process0x15(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t PrimaryWaterPump, WaterPump2, ThreeWayValve, ThreeWayValve2, MixingStep;

  PrimaryWaterPump = Buffer[1];  // 01 when running (Primary Water Pump)
  WaterPump2 = Buffer[4];        // Water Pump 2 Active
  ThreeWayValve = Buffer[6];     // 3 Way Valve Position
  ThreeWayValve2 = Buffer[7];    // 3 Way Valve 2 Position
  MixingStep = Buffer[10];       // Mixing Valve Step

  Status->PrimaryWaterPump = PrimaryWaterPump;
  Status->WaterPump2 = WaterPump2;
  Status->ThreeWayValve = ThreeWayValve;
  Status->ThreeWayValve2 = ThreeWayValve2;
  Status->MixingStep = MixingStep;
}


void ECODANDECODER::Process0x16(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t WaterPump4, WaterPump3, WaterPump13;

  WaterPump4 = Buffer[1];   // DHW Pump connected to CPN4
  WaterPump13 = Buffer[2];  // Zone Pump connected to OUT13
  WaterPump3 = Buffer[3];   // Zone Pump connected to OUT3

  Status->WaterPump4 = WaterPump4;
  Status->WaterPump3 = WaterPump3;
  Status->WaterPump13 = WaterPump13;
}


void ECODANDECODER::Process0x26(uint8_t *Buffer, EcodanStatus *Status) {
  float DHWSetpoint;
  uint8_t SystemPowerMode, SystemOperationMode, HotWaterPowerMode;
  uint8_t HeatingControlModeZone1, HeatingControlModeZone2, HotWaterControlMode;

  Status->LastSystemOperationMode = Status->SystemOperationMode;

  SystemPowerMode = Buffer[3];
  SystemOperationMode = Buffer[4];
  HotWaterControlMode = Buffer[5];
  HeatingControlModeZone1 = Buffer[6];
  HeatingControlModeZone2 = Buffer[7];
  DHWSetpoint = ((float)ExtractUInt16(Buffer, 8) / 100);
  //Zone1FlowSetpoint = ((float)ExtractUInt16(Buffer, 10) / 100);   // Duplicate of 0x09
  //Zone2FlowSetpoint = ((float)ExtractUInt16(Buffer, 12) / 100);   // Duplicate of 0x09

  Status->SystemPowerMode = SystemPowerMode;
  Status->SystemOperationMode = SystemOperationMode;
  Status->HotWaterControlMode = HotWaterControlMode;
  Status->HeatingControlModeZ1 = HeatingControlModeZone1;
  Status->HeatingControlModeZ2 = HeatingControlModeZone2;
  Status->HotWaterSetpoint = DHWSetpoint;
}


void ECODANDECODER::Process0x28(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t HotWaterBoostActive, HolidayModeActive, ProhibitDHW;
  uint8_t ProhibitHeatingZ1, ProhibitHeatingZ2;
  uint8_t ProhibitCoolingZ1, ProhibitCoolingZ2;
  uint8_t SvrControlMode;

  HotWaterBoostActive = Buffer[3];  //Forced DHW Mode (Booster)
  HolidayModeActive = Buffer[4];    // Holiday Mode
  ProhibitDHW = Buffer[5];          //Prohibit DHW (by Timer or Manually)
  ProhibitHeatingZ1 = Buffer[6];    //Prohibit Heating Zone1
  ProhibitCoolingZ1 = Buffer[7];    //Prohibit Cooling Zone1
  ProhibitHeatingZ2 = Buffer[8];    //Prohibit Heating Zone2
  ProhibitCoolingZ2 = Buffer[9];    //Prohibit Cooling Zone2
  SvrControlMode = Buffer[10];      //Server Control Mode

  Status->HotWaterBoostActive = HotWaterBoostActive;  // Hot Water Forced (Boost) Active
  Status->HolidayModeActive = HolidayModeActive;
  Status->ProhibitDHW = ProhibitDHW;  // Check if inhibit or Timer
  Status->ProhibitHeatingZ1 = ProhibitHeatingZ1;
  Status->ProhibitCoolingZ1 = ProhibitCoolingZ1;
  Status->ProhibitHeatingZ2 = ProhibitHeatingZ2;
  Status->ProhibitCoolingZ2 = ProhibitCoolingZ2;
  Status->SvrControlMode = SvrControlMode;
}

void ECODANDECODER::Process0x29(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t HeatCool;
  //float fZone1TempSetpoint, fZone2TempSetpoint;

  HeatCool = Buffer[3];
  //fZone1TempSetpoint = ((float)ExtractUInt16(Buffer, 4) / 100);
  //fZone2TempSetpoint = ((float)ExtractUInt16(Buffer, 6) / 100);

  Status->HeatCool = HeatCool;
  //Status->Zone1TemperatureSetpoint = fZone1TempSetpoint;
  //Status->Zone2TemperatureSetpoint = fZone2TempSetpoint;
}

void ECODANDECODER::Process0xA1(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t Year, Month, Day;
  float ConsumedHeating, ConsumedCooling, ConsumedHotWater;

  Year = Buffer[1];
  Month = Buffer[2];
  Day = Buffer[3];

  ConsumedHeating = ExtractEnergy(Buffer, 4);
  ConsumedCooling = ExtractEnergy(Buffer, 7);
  ConsumedHotWater = ExtractEnergy(Buffer, 10);

  Status->ConsumedDateTimeStamp.tm_year = 100 + Year;
  Status->ConsumedDateTimeStamp.tm_mon = Month - 1;
  Status->ConsumedDateTimeStamp.tm_mday = Day;

  Status->ConsumedHeatingEnergy = ConsumedHeating;
  Status->ConsumedCoolingEnergy = ConsumedCooling;
  Status->ConsumedHotWaterEnergy = ConsumedHotWater;
}

void ECODANDECODER::Process0xA2(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t Year, Month, Day;
  float DeliveredHeating, DeliveredCooling, DeliveredHotWater;

  Year = Buffer[1];
  Month = Buffer[2];
  Day = Buffer[3];

  DeliveredHeating = ExtractEnergy(Buffer, 4);
  DeliveredCooling = ExtractEnergy(Buffer, 7);
  DeliveredHotWater = ExtractEnergy(Buffer, 10);

  Status->DeliveredDateTimeStamp.tm_year = 100 + Year;
  Status->DeliveredDateTimeStamp.tm_mon = Month - 1;
  Status->DeliveredDateTimeStamp.tm_mday = Day;

  Status->DeliveredHeatingEnergy = DeliveredHeating;
  Status->DeliveredCoolingEnergy = DeliveredCooling;
  Status->DeliveredHotWaterEnergy = DeliveredHotWater;
}


void ECODANDECODER::Process0xC9(uint8_t *Buffer, EcodanStatus *Status) {
  uint8_t FTCVersion;

  FTCVersion = Buffer[6];

  Status->FTCVersion = FTCVersion;
}


void ECODANDECODER::WriteOK(uint8_t *Buffer, EcodanStatus *Status) {
  bool Write_To_Ecodan_OK;
  Write_To_Ecodan_OK = true;
  Status->Write_To_Ecodan_OK = Write_To_Ecodan_OK;
}


float ECODANDECODER::ExtractEnergy(uint8_t *Buffer, uint8_t index) {
  float Energy;

  Energy = (float)Buffer[index + 2];
  Energy = Energy / (float)100;
  Energy += (float)ExtractUInt16(Buffer, index);

  return Energy;
}

float ECODANDECODER::ExtractUInt16_Signed(uint8_t *Buffer, uint8_t Index) {
  float Value = (Buffer[Index] << 8) + Buffer[Index + 1];
  return Value;
}

uint16_t ECODANDECODER::ExtractUInt16(uint8_t *Buffer, uint8_t Index) {
  uint16_t Value = (Buffer[Index] << 8) + Buffer[Index + 1];
  return Value;
}

// Used for most single-byte floating point values
float ECODANDECODER::ExtractUInt8_v1(uint8_t *Buffer, uint8_t Index) {
  float Value = (Buffer[Index] / 2) - 40;
  return Value;
}


float ECODANDECODER::ExtractUInt8_v2(uint8_t *Buffer, uint8_t Index) {
  float Value = (Buffer[Index] - 40) / 2;
  return Value;
}


void ECODANDECODER::CreateBlankTxMessage(uint8_t PacketType, uint8_t PayloadSize) {
  CreateBlankMessageTemplate(&TxMessage, PacketType, PayloadSize);
}

void ECODANDECODER::CreateBlankMessageTemplate(MessageStruct *Message, uint8_t PacketType, uint8_t PayloadSize) {
  uint8_t i;

  memset((void *)Message, 0, sizeof(MessageStruct));

  Message->SyncByte = PACKET_SYNC;
  Message->PacketType = PacketType;
  Message->PayloadSize = PayloadSize;
  for (i = 0; i < PREAMBLESIZE; i++) {
    Message->Preamble[i] = Preamble[i];
  }
}

void ECODANDECODER::SetPayloadByte(uint8_t Data, uint8_t Location) {
  TxMessage.Payload[Location] = Data;
}

uint8_t ECODANDECODER::PrepareTxCommand(uint8_t *Buffer) {
  return PrepareCommand(&TxMessage, Buffer);
}

uint8_t ECODANDECODER::PrepareCommand(MessageStruct *Message, uint8_t *Buffer) {
  uint8_t MessageChecksum;
  uint8_t MessageSize;
  uint8_t i;

  Buffer[0] = Message->SyncByte;
  Buffer[1] = Message->PacketType;

  Buffer[2] = Message->Preamble[0];
  Buffer[3] = Message->Preamble[1];

  Buffer[4] = Message->PayloadSize;

  memcpy(&Buffer[5], Message->Payload, Message->PayloadSize);

  MessageSize = HEADERSIZE + Message->PayloadSize;

  MessageChecksum = CheckSum(Buffer, MessageSize);

  Buffer[MessageSize] = MessageChecksum;

  return MessageSize + 1;
}


uint8_t ECODANDECODER::CheckSum(uint8_t *Buffer, uint8_t len) {
  uint8_t sum = 0;
  uint8_t i;

  for (i = 0; i < len; i++) {
    sum += Buffer[i];
  }

  sum = 0xfc - sum;
  sum = sum & 0xff;

  return sum;
}



void ECODANDECODER::EncodeDHWSetpoint(float HotWaterSetpoint) {
  uint8_t UpperByte, LowerByte;
  uint16_t ScaledTarget;

  TxMessage.Payload[0] = TX_MESSAGE_BASIC;
  TxMessage.Payload[1] = SET_HOT_WATER_SETPOINT;

  ScaledTarget = HotWaterSetpoint;
  ScaledTarget *= 100;
  UpperByte = (uint8_t)(ScaledTarget >> 8);
  LowerByte = (uint8_t)(ScaledTarget & 0x00ff);

  TxMessage.Payload[8] = UpperByte;
  TxMessage.Payload[9] = LowerByte;
}


void ECODANDECODER::EncodeControlMode(uint8_t ControlMode, uint8_t Zone) {
  TxMessage.Payload[0] = TX_MESSAGE_BASIC;
  TxMessage.Payload[1] = Zone;
  if (SET_HEATING_CONTROL_MODE_Z1 == Zone) {
    TxMessage.Payload[6] = ControlMode;
  }
  if (SET_HEATING_CONTROL_MODE_Z2 == Zone) {
    TxMessage.Payload[7] = ControlMode;
  }
}


void ECODANDECODER::EncodePower(uint8_t Power) {
  TxMessage.Payload[0] = TX_MESSAGE_BASIC;
  TxMessage.Payload[1] = SET_SYSTEM_POWER;
  TxMessage.Payload[3] = Power;
}


void ECODANDECODER::EncodeRoomThermostat(float Setpoint, uint8_t ControlMode, uint8_t Zone) {
  uint8_t UpperByte, LowerByte;
  uint16_t ScaledTarget;

  ScaledTarget = Setpoint * 100;
  UpperByte = (uint8_t)(ScaledTarget >> 8);
  LowerByte = (uint8_t)(ScaledTarget & 0x00ff);

  TxMessage.Payload[0] = TX_MESSAGE_ROOM_STAT;

  if (ControlMode == HEATING_CONTROL_MODE_COOL_ZONE_TEMP) {
    TxMessage.Payload[3] = ControlMode == HEATING_CONTROL_MODE_COOL_ZONE_TEMP ? 1 : 0;
  }

  if (Zone == ZONE1) {
    TxMessage.Payload[1] = ZONE1_TSTAT;
    TxMessage.Payload[4] = UpperByte;
    TxMessage.Payload[5] = LowerByte;
  } else if (Zone == ZONE2) {
    TxMessage.Payload[1] = ZONE2_TSTAT;
    TxMessage.Payload[6] = UpperByte;
    TxMessage.Payload[7] = LowerByte;
  }
}


void ECODANDECODER::EncodeFlowTemperature(float Setpoint, uint8_t ControlMode, uint8_t Zone) {
  uint8_t UpperByte, LowerByte;
  uint16_t ScaledTarget;

  TxMessage.Payload[0] = TX_MESSAGE_BASIC;
  TxMessage.Payload[2] = Zone;


  if (Zone == ZONE1) {
    TxMessage.Payload[1] = 0x80;
    TxMessage.Payload[6] = ControlMode;  // Thermostat or Flow Setpoint Flag (Thermostat = 0x00, Flow = 0x01/0x03 for Cooling)

    ScaledTarget = Setpoint * 100;
    UpperByte = (uint8_t)(ScaledTarget >> 8);
    LowerByte = (uint8_t)(ScaledTarget & 0x00ff);

    TxMessage.Payload[10] = UpperByte;
    TxMessage.Payload[11] = LowerByte;

  } else if (Zone == ZONE2) {
    TxMessage.Payload[7] = ControlMode;  // Thermostat or Flow Setpoint Flag

    ScaledTarget = Setpoint * 100;
    UpperByte = (uint8_t)(ScaledTarget >> 8);
    LowerByte = (uint8_t)(ScaledTarget & 0x00ff);

    TxMessage.Payload[12] = UpperByte;
    TxMessage.Payload[13] = LowerByte;
  }
}


void ECODANDECODER::EncodeDHWMode(uint8_t Mode) {
  // DHW Mode Normal/Eco
  TxMessage.Payload[0] = TX_MESSAGE_BASIC;
  TxMessage.Payload[1] = SET_HOT_WATER_MODE;
  TxMessage.Payload[5] = Mode;
}

void ECODANDECODER::EncodeForcedDHW(uint8_t OnOff) {
  // DHW Boost Active
  TxMessage.Payload[0] = TX_MESSAGE_CONTROLLER;
  TxMessage.Payload[1] = TX_MESSAGE_SETTING_DHW_Flag;
  TxMessage.Payload[3] = OnOff;
}

void ECODANDECODER::EncodeHolidayMode(uint8_t OnOff) {
  // Holiday Mode Active
  TxMessage.Payload[0] = TX_MESSAGE_CONTROLLER;
  TxMessage.Payload[1] = TX_MESSAGE_SETTING_HOL_Flag;
  TxMessage.Payload[4] = OnOff;
}

void ECODANDECODER::EncodeFTCVersion() {
  // Get FTC Version
  TxMessage.Payload[0] = 0xC9;
  TxMessage.Payload[1] = 0x5F;
}


void ECODANDECODER::EncodeServerControlMode(uint8_t OnOff, uint8_t DHW, uint8_t Z1H, uint8_t Z1C, uint8_t Z2H, uint8_t Z2C) {
  TxMessage.Payload[0] = TX_MESSAGE_CONTROLLER;
  TxMessage.Payload[1] = TX_MESSAGE_SETTING_SRV_Flag | TX_MESSAGE_SETTING_DHW_INH_Flag | TX_MESSAGE_SETTING_HEAT_Z1_INH_Flag | TX_MESSAGE_SETTING_COOL_Z1_INH_Flag | TX_MESSAGE_SETTING_HEAT_Z2_INH_Flag | TX_MESSAGE_SETTING_COOL_Z2_INH_Flag;  // Write the flags where Zones are to be enabled
  TxMessage.Payload[5] = DHW;                                                                                                                                                                                                                    // Write the current status of DHW
  TxMessage.Payload[6] = Z1H;                                                                                                                                                                                                                    // Write the current status of Z1 Heating
  TxMessage.Payload[7] = Z1C;                                                                                                                                                                                                                    // Write the current status of Z1 Cooling
  TxMessage.Payload[8] = Z2H;                                                                                                                                                                                                                    // Write the current status of Z2 Heating
  TxMessage.Payload[9] = Z2C;                                                                                                                                                                                                                    // Write the current status of Z2 Cooling
  TxMessage.Payload[10] = OnOff;                                                                                                                                                                                                                 // Enter or Exit SCM Mode
}


void ECODANDECODER::EncodeProhibit(uint8_t Flags, uint8_t OnOff) {
  // Heating & DHW Inhibit "Server Control" Mode
  TxMessage.Payload[0] = TX_MESSAGE_CONTROLLER;
  TxMessage.Payload[1] = Flags;

  if ((Flags & TX_MESSAGE_SETTING_DHW_INH_Flag) == TX_MESSAGE_SETTING_DHW_INH_Flag) {
    TxMessage.Payload[5] = OnOff;
  }
  if ((Flags & TX_MESSAGE_SETTING_HEAT_Z1_INH_Flag) == TX_MESSAGE_SETTING_HEAT_Z1_INH_Flag) {
    TxMessage.Payload[6] = OnOff;
  }
  if ((Flags & TX_MESSAGE_SETTING_COOL_Z1_INH_Flag) == TX_MESSAGE_SETTING_COOL_Z1_INH_Flag) {
    TxMessage.Payload[7] = OnOff;
  }
  if ((Flags & TX_MESSAGE_SETTING_HEAT_Z2_INH_Flag) == TX_MESSAGE_SETTING_HEAT_Z2_INH_Flag) {
    TxMessage.Payload[8] = OnOff;
  }
  if ((Flags & TX_MESSAGE_SETTING_COOL_Z2_INH_Flag) == TX_MESSAGE_SETTING_COOL_Z2_INH_Flag) {
    TxMessage.Payload[9] = OnOff;
  }
}