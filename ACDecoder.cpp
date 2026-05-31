
#include "ACDecoder.h"
#include <cstdio>
#include <ESPTelnet.h>
extern ESPTelnet TelnetServer;
#include "Debug.h"

uint8_t ACArray0x01[] = {};
uint8_t ACArray0x02[] = {};
uint8_t ACArray0x03[] = {};
uint8_t ACArray0x04[] = {};
uint8_t ACArray0x05[] = {};
uint8_t ACArray0x06[] = {};
uint8_t ACArray0x09[] = {};
uint8_t ACWriteArray0x01[] = {}; // Write CMDs

uint8_t ACBufferArray[][17] = { {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {} };


ACDECODER::ACDECODER(void) {
  memset(&RxMessage, 0, sizeof(ACMessageStruct));
  memset(&Status, 0, sizeof(ACStatus));

  Preamble[0] = 0x01;
  Preamble[1] = 0x30;
}


uint8_t ACDECODER::Process(uint8_t c) {
  uint8_t ReturnValue = false;

  if (BuildRxMessage(&RxMessage, c)) {
    ReturnValue = true;
    if (RxMessage.PacketType == GET_RESPONSE) {
      switch (RxMessage.Payload[0]) {
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
        case 0x06:
          Process0x06(RxMessage.Payload, &Status);
          break;
        case 0x09:
          Process0x09(RxMessage.Payload, &Status);
          break;
      }
    } else if (RxMessage.PacketType == SET_RESPONSE) {
      WriteOK(RxMessage.Payload, &Status);
    } else if (RxMessage.PacketType == CONNECT_RESPONSE) {
    }
  }
  return ReturnValue;
}

uint8_t ACDECODER::BuildRxMessage(ACMessageStruct *Message, uint8_t c) {
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
            DEBUG_PRINTLN("Unknown PacketType");
            BufferPos = 0;
            return false;  // Unknown Packet Type
        }
        break;

      case 2:
        if (c != Preamble[0]) {
          DEBUG_PRINTLN("Preamble 1 Error");
          BufferPos = 0;
          return false;
        }
        break;

      case 3:
        if (c != Preamble[1]) {
          DEBUG_PRINTLN("Preamble 1 Error");
          BufferPos = 0;
          return false;
        }
        break;

      case 4:
        PayloadSize = c;
        if (c > MAXDATABLOCKSIZE) {
          DEBUG_PRINTLN("Oversize Payload");
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
      DEBUG_PRINTLN("CS OK");
      Message->SyncByte = Buffer[0];
      Message->PacketType = Buffer[1];
      Message->Preamble[0] = Buffer[2];
      Message->Preamble[1] = Buffer[3];
      Message->PayloadSize = Buffer[4];
      Message->Checksum = c;
      memcpy(Message->Payload, &Buffer[5], Message->PayloadSize);
      return true;
    } else {
      DEBUG_PRINTLN("Checksum Fail");
      return false;
    }
  }
  return false;
}


void ACDECODER::WriteOK(uint8_t *Buffer, ACStatus *Status) {
  bool Write_To_Ecodan_OK;
  Write_To_Ecodan_OK = true;
  Status->Write_To_Ecodan_OK = Write_To_Ecodan_OK;
}



void ACDECODER::Process0x02(uint8_t *Buffer, ACStatus *Status) {
  for (int i = 1; i < 16; i++) {
    Array0x02[i] = Buffer[i];
  }

  Status->SystemPowerMode = Buffer[3];
  Status->isee = Buffer[4];
  Status->Mode = Buffer[4];
  Status->fan = Buffer[6];
  Status->vane = Buffer[7];
  Status->wideVane = Buffer[10];

  if (Buffer[11] != 0x00) {
    int temp = Buffer[11];
    temp -= 128;
    Status->Temperature = (float)temp / 2;
  } else {
    Status->Temperature = Buffer[5];
    Status->tempMode = true;
  }
}

void ACDECODER::Process0x03(uint8_t *Buffer, ACStatus *Status) {
  for (int i = 1; i < 16; i++) {
    Array0x03[i] = Buffer[i];
  }

  Status->RoomTemp = Buffer[6];
}


void ACDECODER::Process0x04(uint8_t *Buffer, ACStatus *Status) {
  uint8_t CompressorFrequency;

  for (int i = 1; i < 16; i++) {
    Array0x04[i] = Buffer[i];
  }

  // AC
  //TBD
}


void ACDECODER::Process0x05(uint8_t *Buffer, ACStatus *Status) {

  for (int i = 1; i < 16; i++) {
    Array0x05[i] = Buffer[i];
  }

  Status->Timermode = Buffer[3];
  Status->onMinutesSet = Buffer[4];
  Status->onMinutesRemaining = Buffer[6];
  Status->offMinutesSet = Buffer[5];
  Status->offMinutesRemaining = Buffer[7];
}


void ACDECODER::Process0x06(uint8_t *Buffer, ACStatus *Status) {
  for (int i = 1; i < 16; i++) {
    Array0x06[i] = Buffer[i];
  }

  // AC Packet

  uint8_t CompressorFrequency;
  CompressorFrequency = Buffer[3];
  Status->CompressorFrequency = CompressorFrequency;
}



void ACDECODER::Process0x09(uint8_t *Buffer, ACStatus *Status) {
}



void ACDECODER::EncodePower(uint8_t OnOff) {
  // Sample Data
  // fc, 41, 01, 30, 10, 01, 01, __, __, __, __, __, __, __, __, __, __, __, __, __, __, 7c,

  TxMessage.Payload[0] = 0x01;
  TxMessage.Payload[1] = 0x01;   // Power
  TxMessage.Payload[3] = OnOff;  // On/Off
}

void ACDECODER::EncodeSetMode(uint8_t setting) {
  // Sample Data
  //fc, 41, 01, 30, 10, 01, 06, __, __, 01, 02, __, __, __, __, __, __, __, __, __, __, 74, // Mode Auto

  TxMessage.Payload[0] = 0x01;
  TxMessage.Payload[1] = 0x02;  // Mode
  TxMessage.Payload[4] = setting;
}

void ACDECODER::EncodeSetpoint(uint8_t setting, bool floatMode) {
  // Sample Data
  // fc, 41, 01, 30, 10, 01, 04, __, __, __, 02, __, __, __, __, __, __, __, __, __, __, 77,  // 29C

  TxMessage.Payload[0] = 0x01;
  TxMessage.Payload[1] = 0x04;  // Temp
  if (floatMode) {
    TxMessage.Payload[14] = setting;
  } else {
    TxMessage.Payload[5] = (int)setting;
  }
}


void ACDECODER::EncodeFanSpeed(uint8_t setting) {
  // Sample Data
  //fc, 41, 01, 30, 10, 01, 04, __, __, __, 02, __, __, __, __, __, __, __, __, __, __, 77,

  TxMessage.Payload[0] = 0x01;
  TxMessage.Payload[1] = 0x08;  // Fan
  TxMessage.Payload[6] = setting;
}

void ACDECODER::EncodeVane(uint8_t setting) {
  // Sample Data

  TxMessage.Payload[0] = 0x01;
  TxMessage.Payload[1] = 0x10;  // Vane
  TxMessage.Payload[7] = setting;
}

void ACDECODER::EncodeWideVane(uint8_t setting) {
  // Sample Data
  //fc, 41, 01, 30, 10, 01, 10, __, __, __, __, __, __, __, __, __, __, __, __, __, __, 6d,
  TxMessage.Payload[0] = 0x01;
  TxMessage.Payload[1] = 0x00;
  TxMessage.Payload[2] = 0x01;
  TxMessage.Payload[5] = setting;
}


void ACDECODER::EncodeRemoteTemperature(float RemoteTemperature) {
  /*byte packet[PACKET_LEN] = {};

  prepareSetPacket(packet, PACKET_LEN);

  packet[5] = 0x07;
  if (setting > 0) {
    packet[6] = 0x01;
    setting = setting * 2;
    setting = round(setting);
    setting = setting / 2;
    float temp1 = 3 + ((setting - 10) * 2);
    packet[7] = (int)temp1;
    float temp2 = (setting * 2) + 128;
    packet[8] = (int)temp2;
  } else {
    packet[6] = 0x00;
    packet[8] = 0x80;  //MHK1 send 80, even though it could be 00, since ControlByte is 00
  }
  */
}



void ACDECODER::EncodeMELCloud(uint8_t cmd) {
  TxMessage.Payload[0] = cmd;
  for (int i = 1; i < 16; i++) {
    if (cmd == 0x40) {
      TxMessage.Payload[i] = ACWriteArray0x01[i];
    }
  }
}


void ACDECODER::TransfertoBuffer(uint8_t msgtype, uint8_t bufferposition) {
  BufferArray[bufferposition][0] = msgtype;
  for (int i = 1; i < 17; i++) {
    BufferArray[bufferposition][i] = TxMessage.Payload[i - 1];
  }
}

uint8_t ACDECODER::ReturnNextCommandType(uint8_t bufferposition) {
  return BufferArray[bufferposition][0];
}

void ACDECODER::EncodeNextCommand(uint8_t bufferposition) {
  for (int i = 1; i < 17; i++) {
    TxMessage.Payload[i - 1] = BufferArray[bufferposition][i];
  }
}

void ACDECODER::CreateBlankTxMessage(uint8_t PacketType, uint8_t PayloadSize) {
  CreateBlankMessageTemplate(&TxMessage, PacketType, PayloadSize);
}

void ACDECODER::CreateBlankMessageTemplate(ACMessageStruct *Message, uint8_t PacketType, uint8_t PayloadSize) {
  uint8_t i;

  memset((void *)Message, 0, sizeof(ACMessageStruct));

  Message->SyncByte = PACKET_SYNC;
  Message->PacketType = PacketType;
  Message->PayloadSize = PayloadSize;
  for (i = 0; i < PREAMBLESIZE; i++) {
    Message->Preamble[i] = Preamble[i];
  }
}

void ACDECODER::SetPayloadByte(uint8_t Data, uint8_t Location) {
  TxMessage.Payload[Location] = Data;
}

uint8_t ACDECODER::PrepareTxCommand(uint8_t *Buffer) {
  return PrepareCommand(&TxMessage, Buffer);
}

uint8_t ACDECODER::PrepareCommand(ACMessageStruct *Message, uint8_t *Buffer) {
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


uint8_t ACDECODER::CheckSum(uint8_t *Buffer, uint8_t len) {
  uint8_t sum = 0;
  uint8_t i;

  for (i = 0; i < len; i++) {
    sum += Buffer[i];
  }

  sum = 0xfc - sum;
  sum = sum & 0xff;

  return sum;
}

bool ACDECODER::IS_BIT_SET(uint8_t value, uint8_t bit) {
  return (((value) & (1U << (bit))) != 0);
}