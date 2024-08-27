/*
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

// -- Supported Hardware -- //
/* As sold Witty ESP8266 based               / Core 3.1.2 / Flash 4MB (1MB FS / 1MB OTA)                        */
/* ESP32 ESP32 (ESP32S3 Dev Module) / Core 2.0.17 / Flash 4M with SPIFFS (1.2MB APP / 1.5MB SPIFFS)    */
/* ESP32 Ethernet WT32-ETH01                 / Core 3.0.3 / Flash 4MB (1.2MB APP / 1.5MB SPIFFS)                */


#if defined(ESP8266) || defined(ESP32)  // ESP32 or ESP8266 Compatiability

#include <FS.h>                         // Define File System First
#include <LittleFS.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#endif
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESPTelnet.h>
#include "Ecodan.h"


String FirmwareVersion = "v5.2.0";


#ifdef ESP8266                      // Define the Witty ESP8266 Serial Pins
#define HEATPUMP_STREAM SwSerial
#define SERIAL_CONFIG SWSERIAL_8E1
#define RxPin 14
#define TxPin 16
int Activity_LED = 2;
int Reset_Button = 4;
int LDR = A0;
int Red_RGB_LED = 15;
int Green_RGB_LED = 12;
int Blue_RGB_LED = 13;
#endif

#ifdef ESP32              // Define the M5Stack Serial Pins
#include <FastLED.h>
#define FASTLED_INTERNAL
#define NUM_LEDS 1
#define DATA_PIN 35
CRGB leds[NUM_LEDS];
int Reset_Button = 41;
#define HEATPUMP_STREAM Serial2
#define SERIAL_CONFIG SERIAL_8E1
#define RxPin 2
#define TxPin 1
#endif

unsigned long SERIAL_BAUD = 2400;
bool shouldSaveConfig = false;

const int millis_between_write_read = 100;                 // ESP32 Core 3.0.3 this can be as low as 100ms, on Core 2.0.17 
const int clientId_max_length = 25;
const int hostname_max_length = 200;
const int port_max_length = 10;
const int user_max_length = 30;
const int password_max_length = 50;
const int basetopic_max_length = 30;


// The extra parameters to be configured (can be either global or just in the setup)
// After connecting, parameter.getValue() will get you the configured value
// id/name placeholder/prompt default length
// Here you can pre-set the settings for the MQTT connection. The settings can later be changed via Wifi Manager.
struct MqttSettings {
  // These are the placeholder objects for the User
  char clientId[clientId_max_length] = "EcodanBridge";
  char hostname[hostname_max_length] = "IPorDNS";
  char port[port_max_length] = "1883";
  char user[user_max_length] = "Username";
  char password[password_max_length] = "Password";
  char baseTopic[basetopic_max_length] = "Ecodan/ASHP";
  // These are the Index Values for the JSON
  char wm_mqtt_client_id_identifier[20] = "mqtt_client_id";
  char wm_mqtt_hostname_identifier[40] = "mqtt_hostname";
  char wm_mqtt_port_identifier[10] = "mqtt_port";
  char wm_mqtt_user_identifier[20] = "mqtt_user";
  char wm_mqtt_password_identifier[30] = "mqtt_password";
  char wm_mqtt_basetopic_identifier[20] = "mqtt_basetopic";
};

MqttSettings mqttSettings;
ECODAN HeatPump;
#ifdef ESP8266
SoftwareSerial SwSerial;
#endif
WiFiClient NetworkClient;
//WiFiClientSecure NetworkClient;              // Encryption Support
PubSubClient MQTTClient(NetworkClient);
ESPTelnet TelnetServer;
WiFiManager wifiManager;


// Delcare Global Scope for Non-Blocking, always active Portal with "TEMP" placeholder, real values populated later from filesystem
WiFiManagerParameter custom_mqtt_client_id("client_id", "MQTT Client ID", "TEMP", clientId_max_length);
WiFiManagerParameter custom_mqtt_server("server", "MQTT Server", "TEMP", hostname_max_length);
WiFiManagerParameter custom_mqtt_port("port", "MQTT Server Port", "TEMP", port_max_length);
WiFiManagerParameter custom_mqtt_user("user", "MQTT Username", "TEMP", user_max_length);
WiFiManagerParameter custom_mqtt_pass("pass", "MQTT Password", "TEMP", password_max_length);
WiFiManagerParameter custom_mqtt_basetopic("basetopic", "MQTT Base Topic (Default: ASHP/Ecodan)", "TEMP", basetopic_max_length);


#include "TimerCallBack.h"
#include "Debug.h"
#include "MQTTDiscovery.h"
#include "MQTTConfig.h"

void HeatPumpQueryStateEngine(void);
void HeatPumpKeepAlive(void);
void Zone1Report(void);
void Zone2Report(void);
void HotWaterReport(void);
void SystemReport(void);
void AdvancedReport(void);
void AdvancedTwoReport(void);
void EnergyReport(void);
void TriggerFTCVersion(void);

TimerCallBack HeatPumpQuery1(350, HeatPumpQueryStateEngine);  // Set to 500ms (Safe), 320-350ms best time between messages
TimerCallBack HeatPumpQuery2(10000, HeatPumpKeepAlive);       // Set to 10-30s for heat pump query frequency
TimerCallBack HeatPumpQuery3(10800000, TriggerFTCVersion);    // Set to 3hrs for FTC Version Query

unsigned long looppreviousMillis = 0;  // variable for comparing millis counter
unsigned long ftcpreviousMillis = 0;   // variable for comparing millis counter
unsigned long wifipreviousMillis = 0;  // variable for comparing millis counter
int FTCLoopSpeed, CPULoopSpeed;        // variable for holding loop time in ms
bool WiFiOneShot = true;
bool HeatPumpQueryOneShot = true;
bool PostWriteUpdateRequired = false;
float Zone1TemperatureSetpoint_UpdateValue, Zone2TemperatureSetpoint_UpdateValue;
int Zone1FlowSetpoint_UpdateValue, Zone2FlowSetpoint_UpdateValue;





void setup() {
  WiFi.mode(WIFI_STA);                                              // explicitly set mode, esp defaults to STA+AP
  DEBUGPORT.begin(DEBUGBAUD);                                       // Start Debug
  HEATPUMP_STREAM.begin(SERIAL_BAUD, SERIAL_CONFIG, RxPin, TxPin);  // Rx, Tx
  HeatPump.SetStream(&HEATPUMP_STREAM);

  pinMode(Reset_Button, INPUT);  // Pushbutton

// -- Lights for ESP8266 and ESP32 -- //
#ifdef ESP32                                                // Define the M5Stack LED
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);            // ESP32 M5 Stack Atom S3
#endif
#ifdef ESP8266                     // Define the Witty ESP8266 Ports
  pinMode(Activity_LED, OUTPUT);   // ESP8266 Onboard LED
  pinMode(LDR, INPUT);             // LDR
  pinMode(Red_RGB_LED, OUTPUT);    // Red (RGB) LED
  pinMode(Green_RGB_LED, OUTPUT);  // Green (RGB) LED
  pinMode(Blue_RGB_LED, OUTPUT);   // Blue (RGB) LED

  digitalWrite(Activity_LED, HIGH);  // Set On (Inverted)
  digitalWrite(Red_RGB_LED, LOW);    // Set Off
  digitalWrite(Green_RGB_LED, LOW);  // Set Off
  digitalWrite(Blue_RGB_LED, LOW);   // Set Off
#endif

  readSettingsFromConfig();
  initializeWifiManager();
  if (shouldSaveConfig) {
    saveConfig();
  }
  setupTelnet();
  startTelnet();

  MQTTClient.setBufferSize(2048);  // Increase MQTT Buffer Size
  RecalculateMQTTTopics();
  initializeMqttClient();
  MQTTClient.setCallback(MQTTonData);
  wifiManager.startWebPortal();

  HeatPump.Status.Write_To_Ecodan_OK = false;
}


void loop() {
  // -- Loop Start -- //
  looppreviousMillis = millis();  // Loop Speed Check

  // -- Process Handlers -- //
  HeatPumpQuery1.Process();
  HeatPumpQuery2.Process();
  HeatPumpQuery3.Process();
  handleMqttState();
  TelnetServer.loop();
  HeatPump.Process();
  wifiManager.process();

  // -- Config Saver -- //
  if (shouldSaveConfig) { saveConfig(); }                               // Handles WiFiManager Settings Changes

  // -- Heat Pump Write Command Handler -- //
  if (HeatPump.Status.Write_To_Ecodan_OK && PostWriteUpdateRequired) {  // A write command has just been written (Not Keep Alive)
    DEBUG_PRINTLN("Write OK!");                                         // Pause normal processsing until complete    
    HeatPump.Status.Write_To_Ecodan_OK = false;                         // Set back to false
    PostWriteUpdateRequired = false;                                    // Set back to false
    if (MQTTReconnect()) { PublishAllReports(); }                       // Publish update to the MQTT Topics
  }

  // -- WiFi Status Handler -- //
  if (WiFi.status() != WL_CONNECTED) {
#ifdef ESP8266                         // Define the Witty ESP8266 Ports
    digitalWrite(Green_RGB_LED, LOW);  // Turn the Green LED Off
    digitalWrite(Red_RGB_LED, HIGH);   // Turn the Red LED On
#endif
#ifdef ESP32  // Define the M5Stack LED
    leds[0] = CRGB::Black;  // Turn the Green LED Off
    leds[0] = CRGB::Red;    // Turn the Red LED On
    leds[0].fadeLightBy(0);
    FastLED.show();
#endif

    if (WiFiOneShot) {
      wifipreviousMillis = millis();
      WiFiOneShot = false;
    }  // Oneshot to start the timer
    if (millis() - wifipreviousMillis >= 300000) {
#ifdef ESP8266                          // Define the Witty ESP8266 Ports
      digitalWrite(Red_RGB_LED, HIGH);  // Flash the Red LED
      delay(500);
      digitalWrite(Red_RGB_LED, LOW);
      delay(500);
      digitalWrite(Red_RGB_LED, HIGH);
      delay(500);
      digitalWrite(Red_RGB_LED, LOW);
      delay(500);
      digitalWrite(Red_RGB_LED, HIGH);
      ESP.reset();
#endif
#ifdef ESP32  // Define the M5Stack LED
      leds[0] = CRGB::Red;  // Flash the Red LED
      FastLED.show();
      delay(500);
      leds[0] = CRGB::Black;
      FastLED.show();
      delay(500);
      leds[0] = CRGB::Red;
      FastLED.show();
      delay(500);
      leds[0] = CRGB::Black;
      FastLED.show();
      delay(500);
      leds[0] = CRGB::Red;
      FastLED.show();
#endif
#ifdef ESP32
      ESP.restart();
#endif
    }  // Wait for 5 mins to try reconnects then force restart
  } else {
#ifdef ESP8266                       // Define the Witty ESP8266 Ports
    analogWrite(Green_RGB_LED, 30);  // Green LED on, 25% brightness
    digitalWrite(Red_RGB_LED, LOW);  // Turn the Red LED Off
#endif
#ifdef ESP32  // Define the M5Stack LED
    leds[0] = CRGB::Green;
    leds[0].fadeLightBy(200);        // Green LED on, 25% brightness
    FastLED.show();
#endif
  }

  // -- Push Button Action Handler -- //
  if (digitalRead(Reset_Button) == LOW) {  // Inverted (Button Pushed is LOW)
    HeatPump.SetSvrControlMode(0);         // Exit Server Control Mode
#ifdef ESP8266                             // Define the Witty ESP8266 Ports
    digitalWrite(Red_RGB_LED, HIGH);       // Flash the Red LED
    delay(500);
    digitalWrite(Red_RGB_LED, LOW);
    delay(500);
    digitalWrite(Red_RGB_LED, HIGH);
    delay(500);
    digitalWrite(Red_RGB_LED, LOW);
    delay(500);
    digitalWrite(Red_RGB_LED, HIGH);
    delay(500);
    ESP.reset();
#endif
#ifdef ESP32  // Define the M5Stack LED
    leds[0] = CRGB::Red;    // Flash the Red LED
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Red;
    FastLED.show();
    delay(500);
    ESP.restart();
#endif
  }

  // -- Start Sequence -- //
  if (HeatPumpQueryOneShot) {
    HeatPump.GetFTCVersion();
    HeatPumpQueryOneShot = false;
  }

  // -- CPU Loop Time End -- //
  CPULoopSpeed = millis() - looppreviousMillis;  // Loop Speed End Monitor
}

void HeatPumpKeepAlive(void) {
  ftcpreviousMillis = millis();
  HeatPump.KeepAlive();
  HeatPump.TriggerStatusStateMachine();
}

void HeatPumpQueryStateEngine(void) {
  HeatPump.StatusStateMachine();  // Full Read trigged by CurrentMessage

  // Call Once Full Update is complete
  if (HeatPump.UpdateComplete()) {
    DEBUG_PRINTLN("Update Complete");
    FTCLoopSpeed = millis() - ftcpreviousMillis;  // Loop Speed End
    if (MQTTReconnect()) { PublishAllReports(); }
  }
}

void TriggerFTCVersion(void){
  HeatPumpQueryOneShot = true;    // Trigger a read of the FTC version
}

void MQTTonDisconnect(void* response) {
  DEBUG_PRINTLN("MQTT Disconnect");
}

void MQTTonData(char* topic, byte* payload, unsigned int length) {
  payload[length] = 0;
  String Topic = topic;
  String Payload = (char*)payload;

  DEBUG_PRINT("\nReceived MQTT Message on topic ");
  DEBUG_PRINT(Topic.c_str());
  DEBUG_PRINT(" with Payload ");
  DEBUG_PRINTLN(Payload.c_str());

  // Curve or Temp Independent Thermostat Setting
  // Heating & Cooling Zone 1 Commands
  if (Topic == MQTTCommandZone1NoModeSetpoint) {
    MQTTWriteReceived("MQTT Set Zone1 Temperature Setpoint", 6);
    HeatPump.SetZoneTempSetpoint(Payload.toFloat(), HeatPump.Status.HeatingControlModeZ1, ZONE1);
  }
  // Flow Setpoint Commands
  // Heating & Cooling Zone 1 Commands
  if (Topic == MQTTCommandZone1FlowSetpoint) {
    MQTTWriteReceived("MQTT Set Zone1 Flow Setpoint", 6);
    HeatPump.SetFlowSetpoint(Payload.toInt(), HeatPump.Status.HeatingControlModeZ1, ZONE1);
  }

  // Thermostat Setpoint
  // Heating & Cooling Zone 2 Commands
  if (Topic == MQTTCommandZone2NoModeSetpoint) {
    MQTTWriteReceived("MQTT Set Zone2 Temperature Setpoint", 6);
    HeatPump.SetZoneTempSetpoint(Payload.toFloat(), HeatPump.Status.HeatingControlModeZ2, ZONE2);
  }
  // Flow Setpoint Commands
  // Heating & Cooling Zone 2 Commands
  if (Topic == MQTTCommandZone2FlowSetpoint) {
    MQTTWriteReceived("MQTT Set Zone2 Flow Setpoint", 6);
    HeatPump.SetFlowSetpoint(Payload.toInt(), HeatPump.Status.HeatingControlModeZ2, ZONE2);
  }


  // Prohibits for Server Control Mode
  if (Topic == MQTTCommandZone1ProhibitHeating) {
    MQTTWriteReceived("MQTT Zone 1 Prohibit Heating", 16);
    HeatPump.SetProhibits(TX_MESSAGE_SETTING_HEAT_Z1_INH_Flag, Payload.toInt());
    HeatPump.Status.ProhibitHeatingZ1 = Payload.toInt();
  }
  if (Topic == MQTTCommandZone1ProhibitCooling) {
    MQTTWriteReceived("MQTT Zone 1 Prohibit Cooling", 16);
    HeatPump.SetProhibits(TX_MESSAGE_SETTING_COOL_Z1_INH_Flag, Payload.toInt());
    HeatPump.Status.ProhibitCoolingZ1 = Payload.toInt();                          
  }
  if (Topic == MQTTCommandZone2ProhibitHeating) {
    MQTTWriteReceived("MQTT Zone 2 Prohibit Heating", 16);
    HeatPump.SetProhibits(TX_MESSAGE_SETTING_HEAT_Z2_INH_Flag, Payload.toInt());
    HeatPump.Status.ProhibitHeatingZ2 = Payload.toInt();
  }
  if (Topic == MQTTCommandZone2ProhibitCooling) {
    MQTTWriteReceived("MQTT Zone 2 Prohibit Cooling", 16);
    HeatPump.SetProhibits(TX_MESSAGE_SETTING_COOL_Z2_INH_Flag, Payload.toInt());
    HeatPump.Status.ProhibitCoolingZ2 = Payload.toInt();
  }
  if (Topic == MQTTCommandHotwaterProhibit) {
    MQTTWriteReceived("MQTT DHW Prohibit", 16);
    HeatPump.SetProhibits(TX_MESSAGE_SETTING_DHW_INH_Flag, Payload.toInt());
    HeatPump.Status.ProhibitDHW = Payload.toInt();
  }


  // Other Commands
  if (Topic == MQTTCommandHotwaterMode) {
    MQTTWriteReceived("MQTT Set HW Mode", 15);
    HeatPump.SetDHWMode(&Payload);
  }
  if (Topic == MQTTCommandHotwaterBoost) {
    MQTTWriteReceived("MQTT Set HW Boost", 16);
    HeatPump.ForceDHW(Payload.toInt());
    HeatPump.Status.HotWaterBoostActive = Payload.toInt();
  }
  if (Topic == MQTTCommandHotwaterNormalBoost) {
    MQTTWriteReceived("MQTT Normal DHW Boost Run", 16);
    HeatPump.SetProhibits(TX_MESSAGE_SETTING_Normal_DHW_Flag, Payload.toInt());
  }
  if (Topic == MQTTCommandSystemHolidayMode) {
    MQTTWriteReceived("MQTT Set Holiday Mode", 16);
    HeatPump.SetHolidayMode(Payload.toInt());
    HeatPump.Status.HolidayModeActive = Payload.toInt();
  }
  if (Topic == MQTTCommandHotwaterSetpoint) {
    MQTTWriteReceived("MQTT Set HW Setpoint", 6);
    HeatPump.SetHotWaterSetpoint(Payload.toInt());
  }
  if (Topic == MQTTCommandSystemHeatingMode) {
    MQTTWriteReceived("MQTT Set Heating Mode", 4);
    HeatPump.SetHeatingControlMode(&Payload);
  }
  if (Topic == MQTTCommandSystemSvrMode) {
    MQTTWriteReceived("MQTT Server Control Mode", 16);
    HeatPump.SetSvrControlMode(Payload.toInt());
    HeatPump.Status.SvrControlMode = Payload.toInt();
  }
  if (Topic == MQTTCommandSystemPower) {
    MQTTWriteReceived("MQTT Set System Power Mode", 15);
    HeatPump.SetSystemPowerMode(&Payload);
  }
}


void Zone1Report(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  doc[F("Temperature")] = HeatPump.Status.Zone1Temperature;
  doc[F("Setpoint")] = HeatPump.Status.Zone1TemperatureSetpoint;
  doc["HeatingControlMode"] = HeatingControlModeString[HeatPump.Status.HeatingControlModeZ1];
  doc["FSP"] = HeatPump.Status.Zone1FlowTemperatureSetpoint;
  doc["TwoZone_Z1Working"] = HeatPump.Status.TwoZone_Z1Working;
  doc["ProhibitHeating"] = HeatPump.Status.ProhibitHeatingZ1;
  doc["ProhibitCooling"] = HeatPump.Status.ProhibitCoolingZ1;
  doc[F("FlowTemp")] = HeatPump.Status.Zone1FlowTemperature;
  doc[F("ReturnTemp")] = HeatPump.Status.Zone1ReturnTemperature;

  serializeJson(doc, Buffer);

  MQTTClient.publish(MQTT_STATUS_ZONE1.c_str(), Buffer, false);
}

void Zone2Report(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  doc[F("Temperature")] = HeatPump.Status.Zone2Temperature;
  doc[F("Setpoint")] = HeatPump.Status.Zone2TemperatureSetpoint;
  doc["HeatingControlMode"] = HeatingControlModeString[HeatPump.Status.HeatingControlModeZ2];
  doc["FSP"] = HeatPump.Status.Zone2FlowTemperatureSetpoint;
  doc["TwoZone_Z2Working"] = HeatPump.Status.TwoZone_Z2Working;
  doc["ProhibitHeating"] = HeatPump.Status.ProhibitHeatingZ2;
  doc["ProhibitCooling"] = HeatPump.Status.ProhibitCoolingZ2;
  doc[F("FlowTemp")] = HeatPump.Status.Zone2FlowTemperature;
  doc[F("ReturnTemp")] = HeatPump.Status.Zone2ReturnTemperature;

  serializeJson(doc, Buffer);
  MQTTClient.publish(MQTT_STATUS_ZONE2.c_str(), Buffer, false);
}

void HotWaterReport(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  doc[F("Temperature")] = HeatPump.Status.HotWaterTemperature;
  doc[F("TempTHW5A")] = HeatPump.Status.HotWaterTemperatureTHW5A;
  doc[F("Setpoint")] = HeatPump.Status.HotWaterSetpoint;
  doc["HotWaterBoostActive"] = HeatPump.Status.HotWaterBoostActive;
  doc["ProhibitDHW"] = HeatPump.Status.ProhibitDHW;
  doc["DHWActive"] = OFF_ON_String[HeatPump.Status.DHWActive];
  doc["HotWaterControlMode"] = HotWaterControlModeString[HeatPump.Status.HotWaterControlMode];
  doc["LegionellaSetpoint"] = HeatPump.Status.LegionellaSetpoint;
  doc["HotWaterMaxTDrop"] = HeatPump.Status.HotWaterMaximumTempDrop;
  doc["HotWaterPhase"] = DHWPhaseString[HeatPump.Status.DHWHeatSourcePhase];

  serializeJson(doc, Buffer);
  MQTTClient.publish(MQTT_STATUS_HOTWATER.c_str(), Buffer, false);
}

void SystemReport(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  doc[F("HeaterFlow")] = HeatPump.Status.HeaterOutputFlowTemperature;
  doc[F("HeaterReturn")] = HeatPump.Status.HeaterReturnFlowTemperature;
  doc[F("HeaterSetpoint")] = HeatPump.Status.HeaterFlowSetpoint;
  doc[F("OutsideTemp")] = HeatPump.Status.OutsideTemperature;
  doc["Defrost"] = DefrostModeString[HeatPump.Status.Defrost];
  doc["HeaterPower"] = HeatPump.Status.OutputPower;
  doc["Compressor"] = HeatPump.Status.CompressorFrequency;
  doc["SystemPower"] = SystemPowerModeString[HeatPump.Status.SystemPowerMode];
  doc["SystemOperationMode"] = SystemOperationModeString[HeatPump.Status.SystemOperationMode];
  doc["HolidayMode"] = HeatPump.Status.HolidayModeActive;
  doc["FlowRate"] = HeatPump.Status.PrimaryFlowRate;
  doc["RunHours"] = HeatPump.Status.RunHours;

  serializeJson(doc, Buffer);
  MQTTClient.publish(MQTT_STATUS_SYSTEM.c_str(), Buffer, false);
}

void AdvancedReport(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  doc[F("FlowTMax")] = HeatPump.Status.FlowTempMax;
  doc[F("FlowTMin")] = HeatPump.Status.FlowTempMin;
  doc[F("BoilerFlow")] = HeatPump.Status.ExternalBoilerFlowTemperature;
  doc[F("BoilerReturn")] = HeatPump.Status.ExternalBoilerReturnTemperature;
  doc[F("MixingTemp")] = HeatPump.Status.MixingTemperature;
  doc[F("ExternalFlowTemp")] = HeatPump.Status.ExternalFlowTemp;
  doc["Immersion"] = OFF_ON_String[HeatPump.Status.ImmersionActive];
  doc["Booster"] = OFF_ON_String[HeatPump.Status.BoosterActive];
  doc["ThreeWayValve"] = HeatPump.Status.ThreeWayValve;
  doc["PrimaryWaterPump"] = OFF_ON_String[HeatPump.Status.PrimaryWaterPump];
  doc[F("RefrigeTemp")] = HeatPump.Status.RefrigeTemp;
  doc[F("CondensingTemp")] = HeatPump.Status.CondensingTemp;

  serializeJson(doc, Buffer);
  MQTTClient.publish(MQTT_STATUS_ADVANCED.c_str(), Buffer, false);
}


void EnergyReport(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  float heat_cop, cool_cop, dhw_cop, ctotal, dtotal, total_cop;

  // A check for errors before calculating CoP
  if ((HeatPump.Status.DeliveredHeatingEnergy == 0) && (HeatPump.Status.ConsumedHeatingEnergy > 0)) {
    HeatPump.Status.ConsumedHeatingEnergy = 0;  // Re-write
  }
  if ((HeatPump.Status.DeliveredCoolingEnergy == 0) && (HeatPump.Status.ConsumedCoolingEnergy > 0)) {
    HeatPump.Status.ConsumedCoolingEnergy = 0;  // Re-write
  }
  if ((HeatPump.Status.DeliveredHotWaterEnergy == 0) && (HeatPump.Status.ConsumedHotWaterEnergy > 0)) {
    HeatPump.Status.ConsumedHotWaterEnergy = 0;  // Re-write
  }

  // CoP Calculations to avoid divide by 0 occuring
  if (HeatPump.Status.ConsumedHeatingEnergy > 0) {
    heat_cop = HeatPump.Status.DeliveredHeatingEnergy / HeatPump.Status.ConsumedHeatingEnergy;
  } else {
    heat_cop = 0;
  }
  if (HeatPump.Status.ConsumedCoolingEnergy > 0) {
    cool_cop = HeatPump.Status.DeliveredCoolingEnergy / HeatPump.Status.ConsumedCoolingEnergy;
  } else {
    cool_cop = 0;
  }
  if (HeatPump.Status.ConsumedHotWaterEnergy > 0) {
    dhw_cop = (HeatPump.Status.DeliveredHotWaterEnergy / HeatPump.Status.ConsumedHotWaterEnergy);
  } else {
    dhw_cop = 0;
  }

  // CoP Totals
  ctotal = (HeatPump.Status.ConsumedHeatingEnergy + HeatPump.Status.ConsumedCoolingEnergy + HeatPump.Status.ConsumedHotWaterEnergy);
  dtotal = (HeatPump.Status.DeliveredHeatingEnergy + HeatPump.Status.DeliveredCoolingEnergy + HeatPump.Status.DeliveredHotWaterEnergy);

  if (ctotal != 0) {
    total_cop = dtotal / ctotal;
  } else {
    total_cop = 0;
  }


  // Write into the JSON with 2dp rounding

  doc["CHEAT"] = round2(HeatPump.Status.ConsumedHeatingEnergy);
  doc["CCOOL"] = round2(HeatPump.Status.ConsumedCoolingEnergy);
  doc["CDHW"] = round2(HeatPump.Status.ConsumedHotWaterEnergy);
  doc["DHEAT"] = round2(HeatPump.Status.DeliveredHeatingEnergy);
  doc["DCOOL"] = round2(HeatPump.Status.DeliveredCoolingEnergy);
  doc["DDHW"] = round2(HeatPump.Status.DeliveredHotWaterEnergy);
  doc["CTOTAL"] = round2(ctotal);
  doc["DTOTAL"] = round2(dtotal);
  doc["HEAT_CoP"] = round2(heat_cop);
  doc["COOL_CoP"] = round2(cool_cop);
  doc["DHW_CoP"] = round2(dhw_cop);
  doc["TOTAL_COP"] = round2(total_cop);

  serializeJson(doc, Buffer);
  MQTTClient.publish(MQTT_STATUS_ENERGY.c_str(), Buffer, false);
}


void AdvancedTwoReport(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  int ErrorCode = ((String(HeatPump.Status.ErrCode1, HEX)).toInt() * 100) + (String(HeatPump.Status.ErrCode2, HEX)).toInt();

  doc["SvrControlMode"] = HeatPump.Status.SvrControlMode;
  doc["WaterPump2"] = HeatPump.Status.WaterPump2;
  doc["ThreeWayValve2"] = HeatPump.Status.ThreeWayValve2;
  doc["RefrigeFltCode"] = RefrigeFltCodeString[HeatPump.Status.RefrigeFltCode];

  if (ErrorCode == 8000) {
    doc["ErrCode"] = String("None");
  } else {
    doc["ErrCode"] = ErrorCode;
  }

  String FltCodeString = String(FltCodeLetterOne[HeatPump.Status.FltCode1]) + String(FltCodeLetterTwo[HeatPump.Status.FltCode2]);
  if (FltCodeString == "A1") {
    doc["FltCode"] = String("None");
  } else {
    doc["FltCode"] = String(FltCodeString);
  }

  doc["SingleZoneRun"] = HeatPump.Status.SingleZoneParam;
  doc["Z1TstatDemand"] = HeatPump.Status.Zone1ThermostatDemand;
  doc["Z2TstatDemand"] = HeatPump.Status.Zone2ThermostatDemand;
  doc["OTstatDemand"] = HeatPump.Status.OutdoorThermostatDemand;
  doc["OpMode"] = HPControlModeString[HeatPump.Status.HeatCool];

  serializeJson(doc, Buffer);
  MQTTClient.publish(MQTT_STATUS_ADVANCED_TWO.c_str(), Buffer, false);
}

void StatusReport(void) {
  StaticJsonDocument<512> doc;
  char Buffer[512];

  doc["SSID"] = WiFi.SSID();
  doc["RSSI"] = WiFi.RSSI();
  doc["IP"] = WiFi.localIP().toString();
  doc["Firmware"] = FirmwareVersion;  
#ifdef ESP32  // Define the M5Stack LED
  doc["CPUTemp"] = round2(temperatureRead());
#endif
  doc["CPULoopTime"] = CPULoopSpeed;
  doc["FTCLoopTime"] = FTCLoopSpeed;
  doc["FTCVersion"] = FTCString[HeatPump.Status.FTCVersion];

  serializeJson(doc, Buffer);
  MQTTClient.publish(MQTT_STATUS_WIFISTATUS.c_str(), Buffer, false);
  MQTTClient.publish(MQTT_LWT.c_str(), "online");
}

void PublishAllReports(void) {
  Zone1Report();
  Zone2Report();
  HotWaterReport();
  SystemReport();
  AdvancedReport();
  AdvancedTwoReport();
  EnergyReport();
  StatusReport();

  FlashGreenLED();
  DEBUG_PRINTLN("MQTT Published!");
}


void FlashGreenLED(void) {
#ifdef ESP32  // Define the M5Stack LED
  leds[0] = CRGB::Green;    // Flash the Green LED
  leds[0].fadeLightBy(0);
  FastLED.show();
#endif
#ifdef ESP8266                        // Define the Witty ESP8266 Ports
  digitalWrite(Green_RGB_LED, HIGH);  // Flash the Green LED full brightness
#endif
  delay(10);  // Hold for 10ms then WiFi brightness will return it to 25%
}

void setupTelnet() {
  TelnetServer.onConnect(onTelnetConnect);
  TelnetServer.onConnectionAttempt(onTelnetConnectionAttempt);
  TelnetServer.onReconnect(onTelnetReconnect);
  TelnetServer.onDisconnect(onTelnetDisconnect);
}

void startTelnet() {
  DEBUG_PRINT("Telnet: ");
  if (TelnetServer.begin()) {
    DEBUG_PRINTLN("Running");
  } else {
    DEBUG_PRINTLN("error.");
  }
}

void stopTelnet() {
  DEBUG_PRINTLN("Stopping Telnet");
  TelnetServer.stop();
}

void onTelnetConnect(String ip) {
  DEBUG_PRINT("Telnet: ");
  DEBUG_PRINT(ip);
  DEBUG_PRINTLN(" connected");
  TelnetServer.println("\nWelcome " + TelnetServer.getIP());
  TelnetServer.println("(Use ^] + q  to disconnect.)");
}

void onTelnetDisconnect(String ip) {
  DEBUG_PRINT("Telnet: ");
  DEBUG_PRINT(ip);
  DEBUG_PRINTLN(" disconnected");
}

void onTelnetReconnect(String ip) {
  DEBUG_PRINT("Telnet: ");
  DEBUG_PRINT(ip);
  DEBUG_PRINTLN(" reconnected");
}

void onTelnetConnectionAttempt(String ip) {
  DEBUG_PRINT("Telnet: ");
  DEBUG_PRINT(ip);
  DEBUG_PRINTLN(" tried to connected");
}

double round2(double value) {
  return (int)(value * 100 + 0.5) / 100.0;
}

void MQTTWriteReceived(String message, int MsgNumber) {
  DEBUG_PRINTLN(message);
  PostWriteUpdateRequired = true;  // Trigger a read after a write
}

#endif