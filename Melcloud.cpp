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

#define MELCLOUD_NUMBER_COMMANDS 24
uint8_t MELCloudActiveCommand[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x09, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                            0x10, 0x13, 0x14, 0x15, 0x16,
                            0x26, 0x28, 0x29,
                            0xA1, 0xA2,
                            0x00 };

unsigned long mellastmsgdispatchedMillis = 0;  // variable for comparing millis counter

MELCLOUD::MELCLOUD(void)
  : MELCLOUDDECODER() {
  CurrentMessage = 0;
  UpdateFlag = 0;
  Connected = false;
  msbetweenmsg = 0;
}


void MELCLOUD::Process(void) {
  uint8_t c;

  while (DeviceStream->available()) {
    DEBUG_PRINTLN("[MELCloud] ");
    c = DeviceStream->read();

    if (c == 0)
      DEBUG_PRINT("__, ");
    else {
      if (c < 0x10) DEBUG_PRINT("0");
      DEBUG_PRINT(String(c, HEX));
      DEBUG_PRINT(", ");
    }

    if (MELCLOUDDECODER::Process(c)) {
      msbetweenmsg = millis() - mellastmsgdispatchedMillis;
      DEBUG_PRINTLN();
      //DEBUG_PRINT(msbetweenmsg);
      //DEBUG_PRINTLN("ms");
      Connected = true;
    }
  }
}

void MELCLOUD::SetStream(Stream *MELCloudStream) {
  DeviceStream = MELCloudStream;
}



void MELCLOUD::RequestStatus(uint8_t TargetMessage) {
  uint8_t Buffer[COMMANDSIZE];
  uint8_t CommandSize;
  uint8_t i;

  if (TargetMessage == 0xC9) {

  } else {

    MELCLOUDDECODER::CreateBlankTxMessage(GET_RESPONSE, 0x10);
    MELCLOUDDECODER::SetPayloadByte(TargetMessage, 0);


    /*
    MELCLOUDDECODER::SetPayloadByte(0x01, 1);
    MELCLOUDDECODER::SetPayloadByte(0x02, 2);
    MELCLOUDDECODER::SetPayloadByte(0x03, 3);
    MELCLOUDDECODER::SetPayloadByte(0x04, 4);
    MELCLOUDDECODER::SetPayloadByte(0x05, 5);
    MELCLOUDDECODER::SetPayloadByte(0x06, 6);
    MELCLOUDDECODER::SetPayloadByte(0x07, 7);
    MELCLOUDDECODER::SetPayloadByte(0x08, 8);
    MELCLOUDDECODER::SetPayloadByte(0x09, 9);
    MELCLOUDDECODER::SetPayloadByte(0x10, 10);
    MELCLOUDDECODER::SetPayloadByte(0x11, 11);
    MELCLOUDDECODER::SetPayloadByte(0x12, 12);
    MELCLOUDDECODER::SetPayloadByte(0x13, 13);
    MELCLOUDDECODER::SetPayloadByte(0x14, 14);
    MELCLOUDDECODER::::SetPayloadByte(0x15, 15);*/

    if (TargetMessage == 0x02) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 2);
    } else if (TargetMessage == 0x03) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 2);
    } else if (TargetMessage == 0x04) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 2);
    } else if (TargetMessage == 0x05) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x05, 2);
    } else if (TargetMessage == 0x06) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x06, 2);
    } else if (TargetMessage == 0x07) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x07, 2);
    } else if (TargetMessage == 0x08) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x08, 2);
    } else if (TargetMessage == 0x09) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x09, 2);
    } else if (TargetMessage == 0x0B) {
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x10, 2);
    } else if (TargetMessage == 0x0C) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 2);
    } else if (TargetMessage == 0x0D) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 2);
    } else if (TargetMessage == 0x0E) {
      //MELCLOUDDECODER::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::SetPayloadByte(0x03, 2);
    } else if (TargetMessage == 0x0F) {
      //MELCLOUDDECODER::SetPayloadByte(0x01, 4);
      //MELCLOUDDECODER::SetPayloadByte(0x02, 5);
    } else if (TargetMessage == 0x10) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x05, 2);
    } else if (TargetMessage == 0x11) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x06, 2);
    } else if (TargetMessage == 0x12) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x07, 2);
    } else if (TargetMessage == 0x13) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x08, 2);
    } else if (TargetMessage == 0x14) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x09, 2);
    } else if (TargetMessage == 0x15) {
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x10, 2);
    } else if (TargetMessage == 0x16) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 2);
    } else if (TargetMessage == 0x17) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 2);
    } else if (TargetMessage == 0x18) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 2);
    } else if (TargetMessage == 0x19) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 2);
    } else if (TargetMessage == 0x1a) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x05, 2);
    } else if (TargetMessage == 0x1b) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x06, 2);
    } else if (TargetMessage == 0x1c) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x07, 2);
    } else if (TargetMessage == 0x1d) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x08, 2);
    } else if (TargetMessage == 0x1e) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x09, 2);
    } else if (TargetMessage == 0x1f) {
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x10, 2);
    } else if (TargetMessage == 0x20) {
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x01, 2);
    } else if (TargetMessage == 0x26) {
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x02, 2);
    } else if (TargetMessage == 0x27) {
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x03, 2);
    } else if (TargetMessage == 0x28) {
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 2);
    } else if (TargetMessage == 0x29) {
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x05, 2);
    } else if (TargetMessage == 0xA1) {
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x06, 2);
    } else if (TargetMessage == 0xA2) {
      //MELCLOUDDECODER::::SetPayloadByte(0x04, 1);
      //MELCLOUDDECODER::::SetPayloadByte(0x07, 2);
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
  DEBUG_PRINTLN();
}


void MELCLOUD::Connect(void) {
  DEBUG_PRINTLN("[MELCloud] Connecting to MELCloud Device...");
  DeviceStream->write(MELCloudInit3, 7);
  DeviceStream->flush();
  Process();
}


void MELCLOUD::BaudNegotiate(void) {
  DEBUG_PRINTLN("[MELCloud] Negotiating Baud with MELCloud Device...");
  DeviceStream->write(MELCloudInit6, 18);
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
