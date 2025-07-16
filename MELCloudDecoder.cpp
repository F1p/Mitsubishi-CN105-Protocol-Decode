#include "MELCloudDecoder.h"
#include <cstdio>


MELCLOUDDECODER::MELCLOUDDECODER(void) {
  memset(&RxMessage, 0, sizeof(MessageStruct));
  memset(&Status, 0, sizeof(MelCloudStatus));

  Preamble[0] = 0x02;
  Preamble[1] = 0x7a;
}


uint8_t MELCLOUDDECODER::Process(uint8_t c) {
  uint8_t ReturnValue = false;

  if (BuildRxMessage(&RxMessage, c)) {
    ReturnValue = true;
    if (RxMessage.PacketType == GET_REQUEST) {
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
        case 0x06:
          Process0x06(RxMessage.Payload, &Status);
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
        case 0x17:
          Process0x17(RxMessage.Payload, &Status);
          break;
        case 0x18:
          Process0x18(RxMessage.Payload, &Status);
          break;
        case 0x19:
          Process0x19(RxMessage.Payload, &Status);
          break;
        case 0x1a:
          Process0x1a(RxMessage.Payload, &Status);
          break;
        case 0x1b:
          Process0x1b(RxMessage.Payload, &Status);
          break;
        case 0x1c:
          Process0x1c(RxMessage.Payload, &Status);
          break;
        case 0x1d:
          Process0x1d(RxMessage.Payload, &Status);
          break;
        case 0x1e:
          Process0x1e(RxMessage.Payload, &Status);
          break;
        case 0x1f:
          Process0x1f(RxMessage.Payload, &Status);
          break;
        case 0x20:
          Process0x20(RxMessage.Payload, &Status);
          break;
        case 0x26:
          Process0x26(RxMessage.Payload, &Status);
          break;
        case 0x27:
          Process0x27(RxMessage.Payload, &Status);
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
        case 0xa3:
          Process0xA3(RxMessage.Payload, &Status);
          break;
      }
    } else if (RxMessage.PacketType == EXCONNECT_REQUEST) {
      switch (RxMessage.Payload[0]) {
        case 0xc9:
          Process0xC9(RxMessage.Payload, &Status);
          break;
      }
    } else if (RxMessage.PacketType == SET_RESPONSE) {
      WriteOK(RxMessage.Payload, &Status);
    } else if (RxMessage.PacketType == SET_REQUEST) {
      switch (RxMessage.Payload[0]) {
        case 0x32:
          Process0x32(RxMessage.Payload, &Status);
          break;
        case 0x33:
          Process0x33(RxMessage.Payload, &Status);
          break;
        case 0x34:
          Process0x34(RxMessage.Payload, &Status);
          break;
        case 0x35:
          Process0x35(RxMessage.Payload, &Status);
          break;
      }
    } else if (RxMessage.PacketType == CONNECT_REQUEST) {
      Process0x5A(RxMessage.Payload, &Status);
    }
  } else if (CheckForSyncMsg1(&RxMessage, c)) {
    Process0xFF(RxMessage.Payload, &Status, 0);
    ReturnValue = true;
  } else if (CheckForSyncMsg2(&RxMessage, c)) {
    Process0xFF(RxMessage.Payload, &Status, 1);
    ReturnValue = true;
  }
  return ReturnValue;
}


uint8_t MELCLOUDDECODER::CheckForSyncMsg1(MessageStruct *Message, uint8_t c) {
  static uint8_t Buffer[8];
  static uint8_t BufferPos = 0;
  uint8_t i;

  if (BufferPos < 8) {
    switch (BufferPos) {
      case 0:
        if (c != 0x02) return false;
        break;
      case 1:
        if (c != 0xff) return false;
        break;
      case 2:
        if (c != 0xff) return false;
        break;
      case 3:
        if (c != 0x00) return false;
        break;
      case 4:
        if (c != 0x00) return false;
        break;
      case 5:
        if (c != 0x00) return false;
        break;
      case 6:
        if (c != 0x00) return false;
        break;
      case 7:
        if (c != 0x02) return false;
        break;
    }
    Buffer[BufferPos] = c;
    BufferPos++;
    return false;
  }
  Buffer[BufferPos] = c;
  BufferPos = 0;
  memcpy(Message->Payload, &Buffer[0], 8);
  return true;
}

uint8_t MELCLOUDDECODER::CheckForSyncMsg2(MessageStruct *Message, uint8_t c) {
  static uint8_t Buffer[9];
  static uint8_t BufferPos = 0;
  uint8_t i;

  if (BufferPos < 9) {
    switch (BufferPos) {
      case 0:
        if (c != 0x02) return false;
        break;
      case 1:
        if (c != 0xff) return false;
        break;
      case 2:
        if (c != 0xff) return false;
        break;
      case 3:
        if (c != 0x01) return false;
        break;
      case 4:
        if (c != 0x00) return false;
        break;
      case 5:
        if (c != 0x00) return false;
        break;
      case 6:
        if (c != 0x01) return false;
        break;
      case 7:
        if (c != 0x00) return false;
        break;
      case 8:
        if (c != 0x00) return false;
        break;
    }
    Buffer[BufferPos] = c;
    BufferPos++;
    return false;
  }
  Buffer[BufferPos] = c;
  BufferPos = 0;
  memcpy(Message->Payload, &Buffer[0], 9);
  return true;
}

uint8_t MELCLOUDDECODER::BuildRxMessage(MessageStruct *Message, uint8_t c) {
  static uint8_t Buffer[COMMANDSIZE];
  static uint8_t BufferPos = 0;
  static uint8_t PayloadSize = 0;
  uint8_t i;

  if (BufferPos < HEADERSIZE) {
    switch (BufferPos) {
      case 0:
        if (c != PACKET_SYNC) return false;  // Sync Byte 0xFC or MEL Sync 0x02
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
          //Serial.println("Preamble 2 Error");
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
  } else if (BufferPos == (PayloadSize + HEADERSIZE)) {
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


void MELCLOUDDECODER::Process0x5A(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ConnectRequest = true;
}
void MELCLOUDDECODER::Process0x01(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x01;
}
void MELCLOUDDECODER::Process0x02(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x02;
}
void MELCLOUDDECODER::Process0x03(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x03;
}
void MELCLOUDDECODER::Process0x04(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x04;
}
void MELCLOUDDECODER::Process0x05(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x05;
}
void MELCLOUDDECODER::Process0x06(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x06;
}
void MELCLOUDDECODER::Process0x07(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x07;
}
void MELCLOUDDECODER::Process0x08(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x08;
}
void MELCLOUDDECODER::Process0x09(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x09;
}
void MELCLOUDDECODER::Process0x0B(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x0B;
}
void MELCLOUDDECODER::Process0x0C(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x0C;
}
void MELCLOUDDECODER::Process0x0D(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x0D;
}
void MELCLOUDDECODER::Process0x0E(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x0E;
}
void MELCLOUDDECODER::Process0x0F(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x0F;
}
void MELCLOUDDECODER::Process0x10(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x10;
}
void MELCLOUDDECODER::Process0x11(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x11;
}
void MELCLOUDDECODER::Process0x13(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x13;
}
void MELCLOUDDECODER::Process0x14(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x14;
}
void MELCLOUDDECODER::Process0x15(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x15;
}
void MELCLOUDDECODER::Process0x16(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x16;
}
void MELCLOUDDECODER::Process0x17(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x17;
}
void MELCLOUDDECODER::Process0x18(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x18;
}
void MELCLOUDDECODER::Process0x19(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x19;
}
void MELCLOUDDECODER::Process0x1a(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x1a;
}
void MELCLOUDDECODER::Process0x1b(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x1b;
}
void MELCLOUDDECODER::Process0x1c(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x1c;
}
void MELCLOUDDECODER::Process0x1d(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x1d;
}
void MELCLOUDDECODER::Process0x1e(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x1e;
}
void MELCLOUDDECODER::Process0x1f(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x1f;
}
void MELCLOUDDECODER::Process0x20(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x20;
}
void MELCLOUDDECODER::Process0x26(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x26;
}
void MELCLOUDDECODER::Process0x27(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x27;
}
void MELCLOUDDECODER::Process0x28(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x28;
}
void MELCLOUDDECODER::Process0x29(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x29;
}
void MELCLOUDDECODER::Process0xA1(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0xA1;
}
void MELCLOUDDECODER::Process0xA2(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0xA2;
}
void MELCLOUDDECODER::Process0xA3(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0xA3;
}
void MELCLOUDDECODER::Process0xC9(uint8_t *Buffer, MelCloudStatus *Status) {
  Status->ReplyNow = true;
  Status->ActiveMessage = 0xC9;
}
void MELCLOUDDECODER::Process0x32(uint8_t *Buffer, MelCloudStatus *Status) {
  for (int i = 1; i < 16; i++) {
    Array0x32[i] = Buffer[i];
  }
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x32;
}
void MELCLOUDDECODER::Process0x33(uint8_t *Buffer, MelCloudStatus *Status) {
  for (int i = 1; i < 16; i++) {
    Array0x33[i] = Buffer[i];
  }
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x33;
}
void MELCLOUDDECODER::Process0x34(uint8_t *Buffer, MelCloudStatus *Status) {
  if (Buffer[2] == 0x01) {  // MELCloud Connection Heartbeat Bit Toggle Message
    if (Buffer[11] == 0x01) { Status->MEL_Heartbeat = true; }
    Status->MEL_HB_Request = true;
  } else {
    for (int i = 1; i < 16; i++) {
      Array0x34[i] = Buffer[i];
    }
    Status->ReplyNow = true;
    Status->ActiveMessage = 0x34;
  }
}
void MELCLOUDDECODER::Process0x35(uint8_t *Buffer, MelCloudStatus *Status) {
  for (int i = 1; i < 16; i++) {
    Array0x35[i] = Buffer[i];
  }
  Status->ReplyNow = true;
  Status->ActiveMessage = 0x35;
}
void MELCLOUDDECODER::Process0xFF(uint8_t *Buffer, MelCloudStatus *Status, uint8_t type) {
  if (type == 0) {
    Status->MELRequest1 = true;  // Type 1
  } else {
    Status->MELRequest2 = true;  // Type 2
  }
}


void MELCLOUDDECODER::WriteOK(uint8_t *Buffer, MelCloudStatus *Status) {
  bool Write_To_Melcloud_OK;
  Write_To_Melcloud_OK = true;
  Status->Write_To_Melcloud_OK = Write_To_Melcloud_OK;
}


void MELCLOUDDECODER::CreateBlankTxMessage(uint8_t PacketType, uint8_t PayloadSize) {
  CreateBlankMessageTemplate(&TxMessage, PacketType, PayloadSize);
}

void MELCLOUDDECODER::CreateBlankMessageTemplate(MessageStruct *Message, uint8_t PacketType, uint8_t PayloadSize) {
  uint8_t i;

  memset((void *)Message, 0, sizeof(MessageStruct));

  Message->SyncByte = PACKET_SYNC;
  Message->PacketType = PacketType;
  Message->PayloadSize = PayloadSize;
  for (i = 0; i < PREAMBLESIZE; i++) {
    Message->Preamble[i] = Preamble[i];
  }
}

void MELCLOUDDECODER::SetPayloadByte(uint8_t Data, uint8_t Location) {
  TxMessage.Payload[Location] = Data;
}

uint8_t MELCLOUDDECODER::PrepareTxCommand(uint8_t *Buffer) {
  return PrepareCommand(&TxMessage, Buffer);
}

uint8_t MELCLOUDDECODER::PrepareCommand(MessageStruct *Message, uint8_t *Buffer) {
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


uint8_t MELCLOUDDECODER::CheckSum(uint8_t *Buffer, uint8_t len) {
  uint8_t sum = 0;
  uint8_t i;

  for (i = 0; i < len; i++) {
    sum += Buffer[i];
  }

  sum = 0xfc - sum;
  sum = sum & 0xff;

  return sum;
}
