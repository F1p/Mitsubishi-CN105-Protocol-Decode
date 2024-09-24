# Mitsubishi Ecodan Bridge CN105

This software between the Mitsubishi Ecodan FTC controller and a MQTT Broker allows data to be read and commands to be sent.

# Hardware

Supported Hardware is ESP8266 ebay sold hardware: https://www.ebay.co.uk/itm/325967595655



M5Stack Atom S3 Lite

1. Download the files from the [/build](https://github.com/F1p/Mitsubishi-Ecodan-Bridge-CN105/tree/master/build/esp32.esp32.m5stack-atoms3) folder.

2. Download [esptool.exe](https://github.com/espressif/esptool/releases) from Espressif.

3. Place all the files in the same folder, press and hold the side button of the M5Stack, locate the COM port your M5Stack is on

4. Open CMD prompt in the folder with the files saved and run:

    ```
    esptool.exe --chip esp32s3 --port "COM12" --baud 921600  --before default_reset --after hard_reset write_flash -e -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 "ECODAN_Bridge_v5.2.0.bootloader.bin" 0x8000 "ECODAN_Bridge_v5.2.0.partitions.bin" 0xe000 "boot_app0.bin" 0x10000 "ECODAN_Bridge_v5.2.0.bin"
    ```

5. The Pins on the M5Stack are: 1 = Tx, 2 = Rx

Wemos D1 Mini

1. Download the files from the [/build](https://github.com/F1p/Home-Assistant-Mitsubishi-Ecodan-CN105-to-MQTT/tree/master/build/esp8266.esp8266.d1_mini/v5.2.1) folder.

2. Download [upload.py](https://github.com/esp8266/Arduino/blob/master/tools/upload.py) from Github

3. Place all the files in the same folder, locate the COM port your Wemos D1 Mini is on

4.  Open CMD prompt in the folder with the files saved and run:

    ```
    python upload.py --chip esp8266 --port "COM4" --baud "115200" "" erase_flash --before default_reset --after hard_reset write_flash 0x0 "ECODAN_Bridge_v5.2.1_WemosD1.bin"
    ```

5. The Pins on the Wemos D1 Mini are: D0 = Tx, D5 = Rx



# Mitsubishi CN105 Protocol Decode
For Ecodan ASHP Units this is the protocol decode information
https://gitter.im/Mitsubishi-CN105-Protocol-Decode/community


# Physical
Serial, 2400, 8, E, 1
# Command Format
| Header | Payload | Checksum |
|--------|---------|----------|
| 5 Bytes | 16 Bytes | 1 Byte |
## Header
| Sync Byte | Packet Type | Uknown | Unknown | Payload Size |
|---|---|--|---|---|
| 0xfc | Type | 0x02 | 0x7a | Length |
### Sync Byte 
0xfc
### Packet Types
| Value | Packet Type      | Direction      |
|-------|------------------|----------------|
|  0x41 | Set Request      | To Heat Pump   |
|  0x61 | Set Response     | From Heat Pump |
|  0x42 | Get Request      | To Heat Pump   |
|  0x62 | Get Response     | From Heat Pump |
|  0x5A | Connect Request  | To Heat Pump   |
|  0x5B | Get Request      | To Heat Pump   |
|  0x7A | Connect Response | From Heat Pump |
|  0x7B | Get Response     | From Heat Pump |
|  0x5B | Extended Connect Request  | To Heat Pump   |
|  0x7B | Extended Connect Response | To Heat Pump   |
### Length
Payload Size (Bytes)
## Payload
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|---|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| Command | x | x | x | x | x | x | x | x | x | x  | x  |  x |  x |  x |  x |CHK |
## Checksum
Checksum = 0xfc - Sum ( PacketBytes[0..20]) ;
# Set Request - Packet Type 0x41
## Available Commands 
Active commands so far identified.
| Command | Brief Description |
| ------- | ----------- |
| 0x32 | Update Settings |
| 0x34 | Hot Water, Holiday and Prohibits |
| 0x35 | Thermostat Setpoints |
### 0x32 - Set Options
|   0   |   1   |   2   | 3 | 4 |  5  |  6  |  7  |   8   |   9   |  10  |  11  |  12  |  13  | 14 | 15 | 16 |
|-------|-------|-------|---|---|-----|-----|-----|-------|-------|------|------|------|------|----|----|----|
| 0x32  | Flags | Flags | P |Unk| DHW | HC1 | HC2 | DHWSP | DHWSP | Z1SP | Z1SP | Z2SP | Z2SP | MRC| MRC|CHK |  

* Flags (2 Bytes) : Flags to Indicate which fields are active
  * 0x01 0x00 : Set System Power Power
  * 0x02 0x00 : Unknown (Byte 4) - Operation Mode?
  * 0x04 0x00 : Set Hot Water Mode
  * 0x08 0x00 : Set Heating Control Mode Zone 1
  * 0x10 0x00 : Set Heating Control Mode Zone 2
  * 0x20 0x00 : Set Hotwater Setpoint
  * 0x40 0x00 : Not Required (uint16)
  * 0x80 0x00 : Set Zone 1 Setpoints
  * 0x00 0x01 : Not Required (uint16)
  * 0x00 0x02 : Set Zone 2 Setpoints
  * 0x00 0x04 : Not Required (uint16)
  * 0x00 0x08 : MRC Prohibit
  * 0x00 0x10 : Not Required (uint16)
* P : System Power
  * 0x00 : Standby
  * 0x01 : Power On
* DHW : Hot Water Mode
  * 0x00 : Normal
  * 0x01 : Eco
* HC1 / HC2 : Heating Control Mode Zone 1 / 2
  * 0 : Heating Temperature Mode
  * 1 : Heating Flow Control Mode
  * 2 : Heating Compensation Curve Mode
  * 3 : Cooling Temperature Mode
  * 4 : Cooling Flow Control Mode
  * 5 : Dry Up
* DHWSP : Hot Water Setpoint (Temperature * 100)
* Z1SP : Zone 1 Setpoint (* 100)
* Z2SP : Zone 2 Setpoint (* 100)
* MRC Prohibit : (Read 0x26 Byte 14) - **MRC Prohibit command must NOT be written to Shizuoka designed models**
  * 0 : Disable Prohibits
  * 8 : Function Set Prohibit
  * 32 : Setting Temperature Prohibit
  * 64 : Running Mode Prohibit
  * 128 : System On/Off Prohibit

### 0x34 - Hot Water and Holiday Mode
|   0   |  1  |  2  | 3 |   4  |  5   |   6  |  7  |   8   |   9   |  10  |  11  |  12  |  13  | 14 | 15 | 16 |
|-------|-----|-----|---|------|------|------|-----|-------|-------|------|------|------|------|----|----|----|
| 0x34  |Flags|Flags|DHW|  HOL | IDHW | Z1HI | Z1CI| Z2HI  | CZ2I  | SCM  | Unk1 | Unk2 | Unk3 |Unk4|Unk5|CHK |  

* Flags (2 Bytes) : Flags to Indicate which fields are active
  * 0x01 0x00 : Hot Water Force (Boost)
  * 0x02 0x00 : Holiday Mode
  * 0x04 0x00 : DHW Inhibit (Only When in "Server Control Mode")
  * 0x08 0x00 : Heating Z1 Inhibit (Only When in "Server Control Mode")
  * 0x10 0x00 : Cooling Z1 Inhibit (Only When in "Server Control Mode")
  * 0x20 0x00 : Heating Z2 Inhibit (Only When in "Server Control Mode")
  * 0x40 0x00 : Cooling Z2 Inhibit (Only When in "Server Control Mode")
  * 0x80 0x00 : Server Control Mode
  * 0x00 0x01 : Unused 1 (Byte 11)
  * 0x00 0x02 : Unused 2 (Byte 12)
  * 0x00 0x04 : Unused 3 (Byte 13)
  * 0x00 0x08 : Unused 4 (Byte 14)
  * 0x00 0x10 : Unused 5 (Byte 15)
* DHW : On (1) / Off (0)
* HOL : On (1) / Off (0)
* SCM : Server Control Mode On (1) / Off (0)
* IDHW : Inhibit DHW On (1) / Off (0)
* Unk1 : Unused (Sets 0x28 Byte 11)
* Unk2 : Unused (Sets 0x28 Byte 12 - suspected)
* Unk3 : Unused (Sets 0x28 Byte 13 - suspected)
* Unk4 : Unused (Sets 0x28 Byte 14 - suspected)
* Unk5 : Unused (Sets 0x28 Byte 15 - suspected)
### 0x35 - Set Thermostat Setpoints 
|   0   |  1  |  2  | 3  |   4  |  5   |   6  |   7  |   8   |   9   |  10  |  11  |  12  |  13  | 14 | 15 | 16 |
|-------|-----|-----|----|------|------|------|------|-------|-------|------|------|------|------|----|----|----|
| 0x35  |Flags|Flags| CH | Z1SP | Z1SP | Z2SP | Z2SP |       |       |      |      |      |      |    |    |CHK |  

* Flags (2 Bytes) : Flags to Indicate which fields are active
  * 0x01 0x00 : Cooling/Heating
  * 0x02 0x00 : Zone 1 Setpoint
  * 0x04 0x00 : Not Required (uint16)
  * 0x08 0x00 : Zone 2 Setpoint
  * 0x10 0x00 : Not Required (uint16)
  * 0x20 0x00 : Unused (Byte 8)
  * 0x40 0x00 : Unused (Byte 9)
  * 0x80 0x00 : Unused (Byte 10)
  * 0x00 0x01 : Unused (Byte 11)
  * 0x00 0x02 : Unused (Byte 12)
  * 0x00 0x04 : Unused (Byte 13)
  * 0x00 0x08 : Unused (Byte 14)
  * 0x00 0x10 : Unused (Byte 15)
* CH : Cooling (1) /Heating (0)
* Z1SP : Zone 1 Setpoint (* 100)
* Z2SP : Zone 2 Setpoint (* 100)
### 0xC9 - FTC Information
|   0   |   1  | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|------|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0xC9  | 0x5F |   |   |   |   |   |   |   |   |    |    |    |    |    |    |    |  
* 0x5F: Request
# Get Request - Packet Type 0x42
## Available Commands 
Active commands so far identified, 0x00 to 0xff. Commands not listed appear to generate no resaponse. Some command listed have empty, payload 0x00, response.
| Command | Brief Description |
| ------- | ----------- |
| 0x01 | Time & Date |
| 0x02 | Defrost |
| 0x03 | Refrigerant and Zone Running Information |
| 0x04 | Compressor Frequency |
| 0x05 | Hot Water Boot Flag |
| 0x06 | Unknown - Empty Response |
| 0x07 | Output Power |
| 0x09 | Zone 1 & 2 Temperatures and Setpoints, Hot Water Setpoint |
| 0x0b | Zone 1 & 2 and Outside |Temperature
| 0x0c | Water Flow Temperatures |
| 0x0d | Boiler Flow Temperatures |
| 0x0e | Unknown |
| 0x10 | Unknown |
| 0x11 | Unknown |
| 0x13 | Run Hours |
| 0x14 | Primary Flow Rate |
| 0x15 | Unknown Flags |
| 0x16 | Running Pumps |
| 0x17 | Unknown - Empty Response |
| 0x18 | Unknown - Empty Response |
| 0x19 | Unknown - Empty Response |
| 0x1a | Unknown - Empty Response |
| 0x1c | Unknown - Empty Response |
| 0x1d | Unknown - Empty Response |
| 0x1e | Unknown - Empty Response |
| 0x1f | Unknown - Empty Response |
| 0x20 | Unknown - Empty Response |
| 0x26 | Various Operantion Mode Flags |
| 0x27 | Unknown |
| 0x28 | Unknown |
| 0x28 | Various Operantion Mode Flags |
| 0x29 | Zone 1 & 2 Temperatures |
| 0xa1 | Unknown |
| 0xa2 | Unknown |
### Payload - All Commands
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|---|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| Command | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0  | 0  |  0 |  0 |  0 |  0 |  0 |
# Set Response - Packet Type 0x61
## Available Commands 
Active commands so far identified.
| Command | Brief Description |
| ------- | ----------- |
| 0x00 |  OK |
### 0x00 - OK , Command OK, or Just Format?
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|---|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| Command | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0  | 0  |  0 |  0 |  0 |  0 |  0 |
# Get Response - Packet Type 0x62
## Resposes 
Responses so far identified.
| Command | Brief Description |
| ------- | ----------- |
| 0x01 | Time & Date |
| 0x05 | Various Flags |
| 0x09 | Zone 1 & 2 Temperatures and Setpoints, Hot Water Setpoint |
| 0x0b | Zone 1 & 2 and Outside Temperature |
| 0x0c | Water Flow Temperatures |
| 0x0d | Boiler Flow Temperatures |
| 0x26 | Various Operantion Mode Flags |
| 0x28 | Various Operantion Mode Flags |
| 0x29 | Zone 1 & 2 Temperatures |
### 0x01 - Time & Date
|   0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x01  | Y | M | D | h | m | s |   |   |   |    |    |    |    |    |    |    |  
* Y: Year
* M: Month
* D: Day
* h: Hour
* m: Minute
* s: Second
### 0x02 - Defrost
|   0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x02  |   |   | D |   |   |   |   |   |   |    |    |    |    |    |    |    |  
* D: Defrost
### 0x03 - Refrigerant and Zone Running Information
|   0   |  1 | 2  |  3 |  4 |  5 | 6 | 7 |  8  | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|----|----|----|----|----|---|---|-----|---|----|----|----|----|----|----|----|
| 0x03  | RF | F1 | F2 | F3 | F4 |   |   |  M  | S |    |    |    |    |    |    |    |  
* RF: Refrigerant Flt Code
* F1: Fault Code * 100 + Flt Code (F2) (Numbers)
* F3: Fault Code (Letter) 1
* F4: Fault Code (Letter) 2
* M: Multi Zone Running Parameter (3 = Z2 Working, 2 = Z1 Working, 1 = Both Zones working, 0 = Idle)
* S: Single Zone Running Parameter (TBC)?
### 0x04 - Various Flags
|   0   | 1  | 2 | 3 | 4 | 5 | 6 |  7  | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|----|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x04  | CF |   |   |   |   |   |   |   |   |    |    |    |    |    |    |    |  
* CF : Compressor Frequency
### 0x05 - Various Flags
|   0  | 1 | 2 | 3 | 4 |  5 |  6  |  7  | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|------|---|---|---|---|----|-----|-----|---|---|----|----|----|----|----|----|----|
| 0x05 |   |   |   |   | DE | HS  | HW  |   |   |    |    |    |    |    |    |    |  
* DE : Value of 7 given in various running modes
* HS : Heat Source (Suspected) 0 = H/P, 1 = IH, 2 = BH, 3 = IH + BH, 4 = Boiler
* HW : Hot Water Running Mode (0 = Off, 1 = Heat Pump Phase, 2 = Heater Phase (e.g. Immersion or Booster))
### 0x07 - Heater Power
|   0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x07  |   |   |   |   |   | P |   |   |   |    |    |    |    |    |    |    |  
* P : Heater Power (to nearest kW)
### 0x09 - Zone 1 & 2 Temperatures and Setpoints, Hot Water Setpoint
| 0    |   1  |   2  | 3    | 4    | 5    | 6    | 7    | 8    |  9  |  10 |  11 | 12 | 13 | 14 | 15 | 16 |
|------|------|------|------|------|------|------|------|------|-----|-----|-----|----|----|----|----|----|
| 0x09 | Z1T  | Z1T  | Z2T  | Z2T  | Z1ST | Z1SP | Z2SP | Z2SP | LSP | LSP | HWD | FMx | FMn |    |    |    |
* Z1T  : Zone1 Target Temperature * 100
* Z2T  : Zone2 Target Temperature * 100;
* Z1SP : Zone 1 Flow SetFlow Setpoint * 100
* Z2SP : Zone 2 Flow SetFlow Setpoint * 100
* LSP  : Legionella Setpoint * 100
* HWD  : DHW Max Temp Drop -40 / 2
* FMx  : Flow Maximum Temperature -40 / 2
* FMn  : Flow Minimum Temperature -40 / 2 
### 0x0b - Zone 1 & 2 and Outside Temperature
|   0  |  1  |  2  |  3  | 4 | 5 | 6 | 7 |  8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|------|-----|-----|-----|---|---|---|---|----|---|----|----|----|----|----|----|----|
| 0x0b | Z1T | Z1T | Z2T |Z2T| ? | ? |   | RT | RT | CT | O  |    |    |    |    |    |
* Z1T : Zone1 Temperature * 100
* Z2T : Zone2 Temperature * 100
* RT : Refrigerant Temperature * 100 (Where TH2 is installed)
* CT : Condensing Temperature /2 - 40
* O : Outside Temp  /2 - 40
### 0x0c - Heater Flow Temps
|  0   | 1  | 2  | 3 | 4  | 5  | 6 | 7  |  8 | 9 |  10 |  11 | 12 | 13 | 14 | 15 | 16 |
|------|----|----|---|----|----|---|----|----|---|-----|-----|----|----|----|----|----|
| 0x0c | OF | OF |   | RF | RF |   | HW | HW |   | HW2 | HW2 |    |    |    |    |    |
* OF : Heater Water Out Flow  * 100
* RF : Heater Return Flow Temperature * 100
* HW : Hot Water Temperature * 100 (THW5 or THW5B)
* HW2 : Hot Water Temperature * 100 (THW5A if installed)
### 0x0d - Thermistors 1
|  0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x0d | F1 | F1 |   | R1 | R1 |   | F2 | F2 |   |  R2 |  R2  |    |    |    |    |    |
* F1 : Zone 1 Flow Temperature * 100 (Where THW6 installed)
* R1 : Zone 1 Return Temperature * 100  (Where THW7 is installed)
* F1 : Zone 2 Flow Temperature * 100 (Where THW8 installed)
* R1 : Zone 2 Return Temperature * 100  (Where THW9 is installed)
### 0x0e - Thermistors 2
|  0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x0e | F | F |   | R | R |   | X | X |   |    |    |    |    |    |    |    |
* F : Boiler Flow Temperature * 100    Suspected (Where THWB1 installed)
* R : Boiler Return Temperature * 100     Suspected (Where THWB2 is installed)
* X : Mixing Tank Temperature * 100        Suspected (Where THW10 is installed)  
### 0x10 - Hardwired Thermostats
|   0   |  1  |  2  |  3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|-----|-----|----|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x10  | Z1T | Z2T | OT |   |   |   |   |   |   |    |    |    |    |    |    |    |
* Z1T: Hardwired/External (IN1) Thermostat 1 Demand (On/Off)
* Z2T: Hardwired/External (IN6) Thermostat 2 Demand (On/Off)
* OT: Hardwired/External (IN5) Outdoor Thermostat Demand (On/Off)
### 0x13 - Run Hours
|   0   | 1  | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|----|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x13  | U  |   |RH | RH|RH |   |   |   |   |    |    |    |    |    |    |    |  
* U : Unknown
* RH: Run Hours
### 0x14 - Immerson/Booster and Primary Flow Rate
|   0   | 1  | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|----|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x14  |    | B |   |   | I |   |   |   |   |    | PF |    |    |    |    |    |  
* B : Booster Heater Active
* I : Immersion Heater Active
* PF : Primary Flow Rate (l/min)
### 0x15 - Unknown
|   0   | 1  |  2 |  3 |  4 | 5 |  6 |  7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|----|----|----|----|---|----|----|---|---|----|----|----|----|----|----|----|
| 0x15  | PP | U2 | U2 | P2 |   |TWV |TWV2|   |   |    | U5 |    |    |    |    |    |  
* PP : Primary Water Pump (Pump 1/OUT1)
* U2 : Unknown Int (3+4 Two Bytes?)
* P2 : Water Pump 2 (OUT2)
* TWV : Three Way Valve Active
* TWV2 : Three Way Valve 2 Active (Suspected, if installed)
* U5 : Unknown - Always shows 04?
### 0x16 - Pumps Running
|   0   | 1  |  2 |  3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|----|----|----|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0x16  | O4 | O3 | O13|   |   |   |   |   |   |    |    |    |    |    |    |    |  
* O4 : Pump 4 (CPN4)
* O13 : Pump Output (Out13)
* O3 : Pump Output (Out3)
### 0x26 - Operation Information
|  0  | 1 | 2 |  3  | 4  | 5  | 6  |  7 |   8  |  9   |  10 |  11 | 12 | 13 | 14 | 15 | 16 |
|-----|---|---|-----|----|----|----|----|------|------|-----|-----|----|----|----|----|----|
| 0x26|   |   | Pwr | OM | HW |OpZ1|OpZ2| HWSP | HWSP | HSP | HSP | SP | SP | ?? |    |    | 
* Pwr - Power
  * 0 : Standby
  * 1 : On
* OM Operation Mode
  * 0 : Off
  * 1 : Hot Water On
  * 2 : Heating On
  * 5 : Frost Protect
  * 6 : Legionella
* HW - Hot Water Mode
  * 0 : Normal
  * 1 : Economy
* Op - Operation Mode (Zone 1 / Zone 2): 
  * 0 : Heating Temperature Mode
  * 1 : Heating Flow Control Mode
  * 2 : Heating Compensation Curve Mode
  * 3 : Cooling Temperature Mode
  * 4 : Cooling Flow Control Mode
  * 5 : Dry Up
* HWSP : HotWater SetPoint * 100;
* HSP : Heating Flow SetPoint * 100;
* SP : External Flow SetPoint
* ?? : 1 for a few cycles when writing to 0x32 byte 14
### 0x28 - Various Flags
|   0   | 1 | 2 | 3 | 4  | 5  |  6 | 7  |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|----|----|----|----|----|----|----|----|----|----|----|----|----|
| 0x28  |   |   |FHW| HM | HT |PHZ1|PCZ1|PHZ2|PCZ2| SC |    |    |    |    |    |    |
* FHW : Forced DHW Mode Active
* HM : Holiday Mode
* HT : Prohibit DHW
* PHZ1 : Prohibit Heating Zone1
* PCZ1 : Prohibit Cooling Zone1
* PHZ2 : Prohibit Heating Zone2
* PCZ2 : Prohibit Cooling Zone2
* SC : Server Control Mode Active
### 0x29 - Thermostats
|   0   | 1 | 2 | 3 |  4  |  5  |  6  |  7  | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|-----|-----|-----|-----|---|---|----|----|----|----|----|----|----|
| 0x29  |   |   | HC| Z1T | Z1T | Z2T | Z2T |   |   |    |    |    |    |    |    |    |  
* HC : Heating (0) / Cooling (1)
* Z1T : Zone1 Temperature * 100
* Z2T : Zone2 Temperature * 100
### 0xA1 - Consumed Energy
|   0   | 1 | 2 | 3 |  4  |  5  |  6  |  7  | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|-----|-----|-----|-----|---|---|----|----|----|----|----|----|----|
| 0xA1  | Y | M | D | Heat|     |     |Cool |   |   |DHW |    |    |    |    |    |    |  
* Y: Year
* M: Month
* D: Day
### 0xA2 - Delivered Energy
|   0   | 1 | 2 | 3 |  4   |  5  |  6  |  7   | 8 | 9 | 10  | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|------|-----|-----|------|---|---|-----|----|----|----|----|----|----|
| 0xA2  | Y | M | D | Heat |     |     | Cool |   |   | DHW |    |    |    |    |    |    |  
* Y: Year
* M: Month
* D: Day
### 0xC9 - FTC Information
|   0   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
|-------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|
| 0xC9  |U1 |   |U2 |   |U3 | V |   |   |   |    |    |    |    |    |    |    |  
* U1: Unknown
* U2: Unknown
* U3: Unknown
* V: FTC Version:
      0: FTC2B
      1: FTC4
      2: FTC5
      3: FTC6
      4: FTC7
      128: CAHV1A
      129: CAHV1B
      130: CRHV1A
      131: CRHV1B
      132: EAHV1A
      133: EAHV1B
      134: QAHV1A
      135: QAHV1B
      144: PWFY1
