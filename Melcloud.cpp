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
#include "Melcloud.h"

#include <ESPTelnet.h>
extern ESPTelnet TelnetServer;
#include "Debug.h"

uint8_t MELCloudInit3[] = { 0xfc, 0x7a, 0x02, 0x7a, 0x01, 0x00, 0x09 };
uint8_t MELCloudInit6[] = { 0x02, 0xff, 0xff, 0x80, 0x00, 0x00, 0x0A, 0x01, 0x00, 0x40, 0x00, 0x00, 0x06, 0x02, 0x7A, 0x00, 0x00, 0xB5 };
uint8_t MELCloudInit7[] = { 0x02, 0xff, 0xff, 0x81, 0x00, 0x00, 0x00, 0x81 };

bool PrintMELStart = false;
bool FirstReadAfterConnect = false;

MELCLOUD::MELCLOUD(void)
  : MELCLOUDDECODER() {
  UpdateFlag = 0;
  Connected = false;
  msbetweenmsg = 0;
}


void MELCLOUD::Process(void) {
  uint8_t c;

  while (DeviceStream->available()) {
    if (!PrintMELStart) {
      DEBUG_PRINT("[MEL > Bridge] ");
      PrintMELStart = true;
    }
    c = DeviceStream->read();

    if (c == 0)
      DEBUG_PRINT("__, ");
    else {
      if (c < 0x10) DEBUG_PRINT("0");
      DEBUG_PRINT(String(c, HEX));
      DEBUG_PRINT(", ");
    }

    if (MELCLOUDDECODER::Process(c)) {
      DEBUG_PRINTLN();
      PrintMELStart = false;
      Connected = true;
    }
  }
}

void MELCLOUD::SetStream(Stream *MELCloudStream) {
  DeviceStream = MELCloudStream;
}



void MELCLOUD::ReplyStatus(uint8_t TargetMessage) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize;
  uint8_t i;

  DEBUG_PRINT("[Bridge > MEL] ");

  if ((TargetMessage == 0x32) || (TargetMessage == 0x33) || (TargetMessage == 0x34) || (TargetMessage == 0x35)) {
    MELCLOUDDECODER::CreateBlankTxMessage(SET_RESPONSE, 0x10);
  } else if (TargetMessage == 0xC9) {
    MELCLOUDDECODER::CreateBlankTxMessage(EXCONNECT_RESPONSE, 0x10);
  } else {
    MELCLOUDDECODER::CreateBlankTxMessage(GET_RESPONSE, 0x10);
  }

  MELCLOUDDECODER::SetPayloadByte(TargetMessage, 0);

  if (TargetMessage == 0x01) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x01[i], i);
    }
  } else if (TargetMessage == 0x02) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x02[i], i);
    }
  } else if (TargetMessage == 0x03) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x03[i], i);
    }
  } else if (TargetMessage == 0x04) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x04[i], i);
    }
  } else if (TargetMessage == 0x05) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x05[i], i);
    }
  } else if (TargetMessage == 0x06) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x06[i], i);
    }
  } else if (TargetMessage == 0x07) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x07[i], i);
    }
  } else if (TargetMessage == 0x08) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x08[i], i);
    }
  } else if (TargetMessage == 0x09) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x09[i], i);
    }
  } else if (TargetMessage == 0x0B) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x0b[i], i);
    }
  } else if (TargetMessage == 0x0C) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x0c[i], i);
    }
  } else if (TargetMessage == 0x0D) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x0d[i], i);
    }
  } else if (TargetMessage == 0x0E) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x0e[i], i);
    }
  } else if (TargetMessage == 0x0F) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x0f[i], i);
    }
  } else if (TargetMessage == 0x10) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x10[i], i);
    }
  } else if (TargetMessage == 0x11) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x11[i], i);
    }
  } else if (TargetMessage == 0x12) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x12[i], i);
    }
  } else if (TargetMessage == 0x13) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x13[i], i);
    }
  } else if (TargetMessage == 0x14) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x14[i], i);
    }
  } else if (TargetMessage == 0x15) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x15[i], i);
    }
  } else if (TargetMessage == 0x16) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x16[i], i);
    }
  } else if (TargetMessage == 0x17) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x17[i], i);
    }
  } else if (TargetMessage == 0x18) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x18[i], i);
    }
  } else if (TargetMessage == 0x19) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x19[i], i);
    }
  } else if (TargetMessage == 0x1a) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x1a[i], i);
    }
  } else if (TargetMessage == 0x1b) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x1b[i], i);
    }
  } else if (TargetMessage == 0x1c) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x1c[i], i);
    }
  } else if (TargetMessage == 0x1d) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x1d[i], i);
    }
  } else if (TargetMessage == 0x1e) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x1e[i], i);
    }
  } else if (TargetMessage == 0x1f) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x1f[i], i);
    }
  } else if (TargetMessage == 0x20) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x20[i], i);
    }
  } else if (TargetMessage == 0x26) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x26[i], i);
    }
  } else if (TargetMessage == 0x27) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x27[i], i);
    }
  } else if (TargetMessage == 0x28) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x28[i], i);
    }
    if (FirstReadAfterConnect) {
      FirstReadAfterConnect = false;
      MELCLOUDDECODER::SetPayloadByte(0x01, 11);
    }
  } else if (TargetMessage == 0x29) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0x29[i], i);
    }
  } else if (TargetMessage == 0xA1) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0xa1[i], i);
    }
  } else if (TargetMessage == 0xA2) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0xa2[i], i);
    }
  } else if (TargetMessage == 0xA3) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0xa3[i], i);
    }
  } else if ((TargetMessage == 0x32) | (TargetMessage == 0x33) | (TargetMessage == 0x34) | (TargetMessage == 0x35)) {
    MELCLOUDDECODER::SetPayloadByte(0x00, 0);  // Ok Message reply to writes
  } else if (TargetMessage == 0xC9) {
    for (int i = 1; i < 16; i++) {
      MELCLOUDDECODER::SetPayloadByte(Array0xc9[i], i);
    }
  }


  CommandSize = MELCLOUDDECODER::PrepareTxCommand(Buffer);
  DeviceStream->write(Buffer, CommandSize);
  DeviceStream->flush();


  for (i = 0; i < CommandSize; i++) {
    if (Buffer[i] < 0x10) DEBUG_PRINT("0");
    DEBUG_PRINT(String(Buffer[i], HEX));
    DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}


void MELCLOUD::Connect(void) {
  DEBUG_PRINTLN("[Bridge > MEL] Connecting to MELCloud Device...");
  FirstReadAfterConnect = true;
  DeviceStream->write(MELCloudInit3, 7);
  DeviceStream->flush();
  Process();
}


void MELCLOUD::MELNegotiate1(void) {
  DEBUG_PRINTLN("[Bridge > MEL] Negotiating First with MELCloud Device...");
  DeviceStream->write(MELCloudInit6, 18);
  DeviceStream->flush();
  Process();
}


void MELCLOUD::MELNegotiate2(void) {
  DEBUG_PRINTLN("[Bridge > MEL] Negotiating Second with MELCloud Device...");
  DeviceStream->write(MELCloudInit7, 8);
  DeviceStream->flush();
  Process();
}


uint8_t MELCLOUD::UpdateComplete(void) {
  if (UpdateFlag) {
    UpdateFlag = 0;
    return 1;
  } else {
    return 0;
  }
}

uint8_t MELCLOUD::Lastmsbetweenmsg(void) {
  return msbetweenmsg;
}
