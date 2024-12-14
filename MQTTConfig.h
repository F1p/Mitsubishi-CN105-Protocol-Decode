String MQTT_BASETOPIC = "Ecodan/ASHP";

String MQTT_LWT = MQTT_BASETOPIC + "/LWT";
String MQTT_STATUS = MQTT_BASETOPIC + "/Status";
String MQTT_COMMAND = MQTT_BASETOPIC + "/Command";

String MQTT_STATUS_ZONE1 = MQTT_STATUS + "/Zone1";
String MQTT_STATUS_ZONE2 = MQTT_STATUS + "/Zone2";
String MQTT_STATUS_HOTWATER = MQTT_STATUS + "/HotWater";
String MQTT_STATUS_SYSTEM = MQTT_STATUS + "/System";
String MQTT_STATUS_ADVANCED = MQTT_STATUS + "/Advanced";
String MQTT_STATUS_ADVANCED_TWO = MQTT_STATUS + "/AdvancedTwo";
String MQTT_STATUS_ENERGY = MQTT_STATUS + "/Energy";
String MQTT_STATUS_WIFISTATUS = MQTT_STATUS + "/WiFiStatus";

String MQTT_COMMAND_ZONE1 = MQTT_COMMAND + "/Zone1";
String MQTT_COMMAND_ZONE2 = MQTT_COMMAND + "/Zone2";
String MQTT_COMMAND_HOTWATER = MQTT_COMMAND + "/HotWater";
String MQTT_COMMAND_SYSTEM = MQTT_COMMAND + "/System";

String MQTT_COMMAND_ZONE1_FLOW_SETPOINT = MQTT_COMMAND_ZONE1 + "/FlowSetpoint";
String MQTT_COMMAND_ZONE1_NOMODE_SETPOINT = MQTT_COMMAND_ZONE1 + "/ThermostatSetpoint";
String MQTT_COMMAND_ZONE1_HEAT_PROHIBIT = MQTT_COMMAND_ZONE1 + "/ProhibitHeating";
String MQTT_COMMAND_ZONE1_COOL_PROHIBIT = MQTT_COMMAND_ZONE1 + "/ProhibitCooling";
String MQTT_COMMAND_ZONE1_HEATINGMODE = MQTT_COMMAND_ZONE1 + "/HeatingMode";

String MQTT_COMMAND_ZONE2_FLOW_SETPOINT = MQTT_COMMAND_ZONE2 + "/FlowSetpoint";
String MQTT_COMMAND_ZONE2_NOMODE_SETPOINT = MQTT_COMMAND_ZONE2 + "/ThermostatSetpoint";
String MQTT_COMMAND_ZONE2_HEAT_PROHIBIT = MQTT_COMMAND_ZONE2 + "/ProhibitHeating";
String MQTT_COMMAND_ZONE2_COOL_PROHIBIT = MQTT_COMMAND_ZONE2 + "/ProhibitCooling";
String MQTT_COMMAND_ZONE2_HEATINGMODE = MQTT_COMMAND_ZONE2 + "/HeatingMode";

String MQTT_COMMAND_HOTWATER_MODE = MQTT_COMMAND_HOTWATER + "/Mode";
String MQTT_COMMAND_HOTWATER_SETPOINT = MQTT_COMMAND_HOTWATER + "/Setpoint";
String MQTT_COMMAND_HOTWATER_BOOST = MQTT_COMMAND_HOTWATER + "/Boost";
String MQTT_COMMAND_HOTWATER_NORM_BOOST = MQTT_COMMAND_HOTWATER + "/NormalBoost";
String MQTT_COMMAND_HOTWATER_PROHIBIT = MQTT_COMMAND_HOTWATER + "/Prohibit";

String MQTT_COMMAND_SYSTEM_HOLIDAYMODE = MQTT_COMMAND_SYSTEM + "/HolidayMode";
String MQTT_COMMAND_SYSTEM_SVRMODE = MQTT_COMMAND_SYSTEM + "/SvrControlMode";
String MQTT_COMMAND_SYSTEM_POWER = MQTT_COMMAND_SYSTEM + "/Power";

String MQTTCommandZone1FlowSetpoint = MQTT_COMMAND_ZONE1_FLOW_SETPOINT;
String MQTTCommandZone1NoModeSetpoint = MQTT_COMMAND_ZONE1_NOMODE_SETPOINT;
String MQTTCommandZone1ProhibitHeating = MQTT_COMMAND_ZONE1_HEAT_PROHIBIT;
String MQTTCommandZone1ProhibitCooling = MQTT_COMMAND_ZONE1_COOL_PROHIBIT;
String MQTTCommandZone1HeatingMode = MQTT_COMMAND_ZONE1_HEATINGMODE;

String MQTTCommandZone2FlowSetpoint = MQTT_COMMAND_ZONE2_FLOW_SETPOINT;
String MQTTCommandZone2NoModeSetpoint = MQTT_COMMAND_ZONE2_NOMODE_SETPOINT;
String MQTTCommandZone2ProhibitHeating = MQTT_COMMAND_ZONE2_HEAT_PROHIBIT;
String MQTTCommandZone2ProhibitCooling = MQTT_COMMAND_ZONE2_COOL_PROHIBIT;
String MQTTCommandZone2HeatingMode = MQTT_COMMAND_ZONE2_HEATINGMODE;

String MQTTCommandHotwaterMode = MQTT_COMMAND_HOTWATER_MODE;
String MQTTCommandHotwaterSetpoint = MQTT_COMMAND_HOTWATER_SETPOINT;
String MQTTCommandHotwaterBoost = MQTT_COMMAND_HOTWATER_BOOST;
String MQTTCommandHotwaterNormalBoost = MQTT_COMMAND_HOTWATER_NORM_BOOST;
String MQTTCommandHotwaterProhibit = MQTT_COMMAND_HOTWATER_PROHIBIT;

String MQTTCommandSystemHolidayMode = MQTT_COMMAND_SYSTEM_HOLIDAYMODE;
String MQTTCommandSystemSvrMode = MQTT_COMMAND_SYSTEM_SVRMODE;
String MQTTCommandSystemPower = MQTT_COMMAND_SYSTEM_POWER;

char MQTT_ClientID[20] = "";
char DeviceID[15] = "";
const char ClientPrefix[14] = "EcodanBridge-";
char WiFiHostname[26] = "";

// Programs

#if defined(ESP8266) || defined(ESP32)  // ESP32 or ESP8266 Compatiability
void readSettingsFromConfig() {
  // Clean LittleFS for testing
  //LittleFS.format();

  // Read configuration from LittleFS JSON
  DEBUG_PRINTLN("Mounting File System...");
#ifdef ESP8266
  if (LittleFS.begin()) {
#endif
#ifdef ESP32
    if (LittleFS.begin("/storage")) {
#endif
      DEBUG_PRINTLN("Mounted File System");
      if (LittleFS.exists("/config.json")) {
        //file exists, reading and loading
        DEBUG_PRINTLN("Reading config file");
        File configFile = LittleFS.open("/config.json", "r");
        if (configFile) {
          DEBUG_PRINTLN("Opened config file");
          JsonDocument doc;
          DeserializationError error = deserializeJson(doc, configFile);
          if (error) {
            DEBUG_PRINT("Failed to read file: ");
            DEBUG_PRINTLN(error.c_str());
          } else {
            DEBUG_PRINTLN("Parsed JSON: ");
            serializeJson(doc, Serial);
            DEBUG_PRINTLN();

            // Build in safety check, otherwise ESP will crash out and you can't get back in
            if (doc.containsKey(mqttSettings.wm_device_id_identifier)) {
              if ((sizeof(doc[mqttSettings.wm_device_id_identifier]) > 0) && ((sizeof(doc[mqttSettings.wm_device_id_identifier]) + 1) <= deviceId_max_length)) {
                strcpy(mqttSettings.deviceId, doc[mqttSettings.wm_device_id_identifier]);
              }
            } else {  // For upgrading from <5.3.1, create the entry
#ifdef ESP8266
              snprintf(DeviceID, 15, (String(ESP.getChipId(), HEX)).c_str());
#endif
#ifdef ESP32
              snprintf(DeviceID, 15, (String(ESP.getEfuseMac(), HEX)).c_str());
#endif
              strcpy(mqttSettings.deviceId, DeviceID);
              shouldSaveConfig = true;  // Save config after exit to update the file
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_client_id_identifier)) {
              if ((sizeof(doc[mqttSettings.wm_mqtt_client_id_identifier]) > 0) && ((sizeof(doc[mqttSettings.wm_mqtt_client_id_identifier]) + 1) <= clientId_max_length)) {
                strcpy(mqttSettings.clientId, doc[mqttSettings.wm_mqtt_client_id_identifier]);
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_hostname_identifier)) {
              if ((sizeof(doc[mqttSettings.wm_mqtt_hostname_identifier]) > 0) && ((sizeof(doc[mqttSettings.wm_mqtt_hostname_identifier]) + 1) <= hostname_max_length)) {
                strcpy(mqttSettings.hostname, doc[mqttSettings.wm_mqtt_hostname_identifier]);
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_port_identifier)) {
              if ((sizeof(doc[mqttSettings.wm_mqtt_port_identifier]) > 0) && ((sizeof(doc[mqttSettings.wm_mqtt_port_identifier]) + 1) <= port_max_length)) {
                strcpy(mqttSettings.port, doc[mqttSettings.wm_mqtt_port_identifier]);
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_user_identifier)) {
              if ((sizeof(doc[mqttSettings.wm_mqtt_user_identifier]) > 0) && ((sizeof(doc[mqttSettings.wm_mqtt_user_identifier]) + 1) <= user_max_length)) {
                strcpy(mqttSettings.user, doc[mqttSettings.wm_mqtt_user_identifier]);
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_password_identifier)) {
              if ((sizeof(doc[mqttSettings.wm_mqtt_password_identifier]) > 0) && ((sizeof(doc[mqttSettings.wm_mqtt_password_identifier]) + 1) <= password_max_length)) {
                strcpy(mqttSettings.password, doc[mqttSettings.wm_mqtt_password_identifier]);
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_basetopic_identifier)) {
              if ((sizeof(doc[mqttSettings.wm_mqtt_basetopic_identifier]) > 0) && ((sizeof(doc[mqttSettings.wm_mqtt_basetopic_identifier]) + 1) <= basetopic_max_length)) {
                strcpy(mqttSettings.baseTopic, doc[mqttSettings.wm_mqtt_basetopic_identifier]);
                MQTT_BASETOPIC = mqttSettings.baseTopic;
              }
            }
          }
        }
        configFile.close();
      } else {
        DEBUG_PRINTLN("No config file exists, use placeholder values");

        // Populate the Dynamic Variables (Device ID)
#ifdef ESP8266
        snprintf(DeviceID, 15, (String(ESP.getChipId(), HEX)).c_str());
#endif
#ifdef ESP32
        snprintf(DeviceID, 15, (String(ESP.getEfuseMac(), HEX)).c_str());
#endif
        snprintf(MQTT_ClientID, 20, "%s%s", ClientPrefix, DeviceID);
        strcpy(mqttSettings.clientId, MQTT_ClientID);
        strcpy(mqttSettings.deviceId, DeviceID);
      }
    } else {
      DEBUG_PRINTLN("Failed to mount File System");
    }
  }



  void RecalculateMQTTTopics() {
    // The base topic may change via WiFi Manager
    MQTT_LWT = MQTT_BASETOPIC + "/LWT";
    MQTT_STATUS = MQTT_BASETOPIC + "/Status";
    MQTT_COMMAND = MQTT_BASETOPIC + "/Command";

    MQTT_STATUS_ZONE1 = MQTT_STATUS + "/Zone1";
    MQTT_STATUS_ZONE2 = MQTT_STATUS + "/Zone2";
    MQTT_STATUS_HOTWATER = MQTT_STATUS + "/HotWater";
    MQTT_STATUS_SYSTEM = MQTT_STATUS + "/System";
    MQTT_STATUS_ADVANCED = MQTT_STATUS + "/Advanced";
    MQTT_STATUS_ADVANCED_TWO = MQTT_STATUS + "/AdvancedTwo";
    MQTT_STATUS_ENERGY = MQTT_STATUS + "/Energy";
    MQTT_STATUS_WIFISTATUS = MQTT_STATUS + "/WiFiStatus";

    MQTT_COMMAND_ZONE1 = MQTT_COMMAND + "/Zone1";
    MQTT_COMMAND_ZONE2 = MQTT_COMMAND + "/Zone2";
    MQTT_COMMAND_HOTWATER = MQTT_COMMAND + "/HotWater";
    MQTT_COMMAND_SYSTEM = MQTT_COMMAND + "/System";

    MQTT_COMMAND_ZONE1_FLOW_SETPOINT = MQTT_COMMAND_ZONE1 + "/FlowSetpoint";
    MQTT_COMMAND_ZONE1_NOMODE_SETPOINT = MQTT_COMMAND_ZONE1 + "/ThermostatSetpoint";
    MQTT_COMMAND_ZONE1_HEAT_PROHIBIT = MQTT_COMMAND_ZONE1 + "/ProhibitHeating";
    MQTT_COMMAND_ZONE1_COOL_PROHIBIT = MQTT_COMMAND_ZONE1 + "/ProhibitCooling";
    MQTT_COMMAND_ZONE1_HEATINGMODE = MQTT_COMMAND_ZONE1 + "/HeatingMode";

    MQTT_COMMAND_ZONE2_FLOW_SETPOINT = MQTT_COMMAND_ZONE2 + "/FlowSetpoint";
    MQTT_COMMAND_ZONE2_NOMODE_SETPOINT = MQTT_COMMAND_ZONE2 + "/ThermostatSetpoint";
    MQTT_COMMAND_ZONE2_HEAT_PROHIBIT = MQTT_COMMAND_ZONE2 + "/ProhibitHeating";
    MQTT_COMMAND_ZONE2_COOL_PROHIBIT = MQTT_COMMAND_ZONE2 + "/ProhibitCooling";
    MQTT_COMMAND_ZONE2_HEATINGMODE = MQTT_COMMAND_ZONE2 + "/HeatingMode";

    MQTT_COMMAND_HOTWATER_MODE = MQTT_COMMAND_HOTWATER + "/Mode";
    MQTT_COMMAND_HOTWATER_SETPOINT = MQTT_COMMAND_HOTWATER + "/Setpoint";
    MQTT_COMMAND_HOTWATER_BOOST = MQTT_COMMAND_HOTWATER + "/Boost";
    MQTT_COMMAND_HOTWATER_NORM_BOOST = MQTT_COMMAND_HOTWATER + "/NormalBoost";
    MQTT_COMMAND_HOTWATER_PROHIBIT = MQTT_COMMAND_HOTWATER + "/Prohibit";

    MQTT_COMMAND_SYSTEM_HOLIDAYMODE = MQTT_COMMAND_SYSTEM + "/HolidayMode";
    MQTT_COMMAND_SYSTEM_SVRMODE = MQTT_COMMAND_SYSTEM + "/SvrControlMode";
    MQTT_COMMAND_SYSTEM_POWER = MQTT_COMMAND_SYSTEM + "/Power";

    MQTTCommandZone1FlowSetpoint = MQTT_COMMAND_ZONE1_FLOW_SETPOINT;
    MQTTCommandZone1NoModeSetpoint = MQTT_COMMAND_ZONE1_NOMODE_SETPOINT;
    MQTTCommandZone1ProhibitHeating = MQTT_COMMAND_ZONE1_HEAT_PROHIBIT;
    MQTTCommandZone1ProhibitCooling = MQTT_COMMAND_ZONE1_COOL_PROHIBIT;
    MQTTCommandZone1HeatingMode = MQTT_COMMAND_ZONE1_HEATINGMODE;

    MQTTCommandZone2FlowSetpoint = MQTT_COMMAND_ZONE2_FLOW_SETPOINT;
    MQTTCommandZone2NoModeSetpoint = MQTT_COMMAND_ZONE2_NOMODE_SETPOINT;
    MQTTCommandZone2ProhibitHeating = MQTT_COMMAND_ZONE2_HEAT_PROHIBIT;
    MQTTCommandZone2ProhibitCooling = MQTT_COMMAND_ZONE2_COOL_PROHIBIT;
    MQTTCommandZone2HeatingMode = MQTT_COMMAND_ZONE2_HEATINGMODE;

    MQTTCommandHotwaterMode = MQTT_COMMAND_HOTWATER_MODE;
    MQTTCommandHotwaterSetpoint = MQTT_COMMAND_HOTWATER_SETPOINT;
    MQTTCommandHotwaterBoost = MQTT_COMMAND_HOTWATER_BOOST;
    MQTTCommandHotwaterNormalBoost = MQTT_COMMAND_HOTWATER_NORM_BOOST;
    MQTTCommandHotwaterProhibit = MQTT_COMMAND_HOTWATER_PROHIBIT;

    MQTTCommandSystemHolidayMode = MQTT_COMMAND_SYSTEM_HOLIDAYMODE;
    MQTTCommandSystemSvrMode = MQTT_COMMAND_SYSTEM_SVRMODE;
    MQTTCommandSystemPower = MQTT_COMMAND_SYSTEM_POWER;
  }




  void saveConfig() {
    // Read MQTT Portal Values for save to file system
    DEBUG_PRINTLN("Copying Portal Values...");
    strcpy(mqttSettings.deviceId, custom_device_id.getValue());
    strcpy(mqttSettings.clientId, custom_mqtt_client_id.getValue());
    strcpy(mqttSettings.hostname, custom_mqtt_server.getValue());
    strcpy(mqttSettings.port, custom_mqtt_port.getValue());
    strcpy(mqttSettings.user, custom_mqtt_user.getValue());
    strcpy(mqttSettings.password, custom_mqtt_pass.getValue());
    strcpy(mqttSettings.baseTopic, custom_mqtt_basetopic.getValue());

    DEBUG_PRINT("Saving config... ");
    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile) {
      DEBUG_PRINTLN("[FAILED] Unable to open config file for writing");
    } else {
      JsonDocument doc;
      doc[mqttSettings.wm_device_id_identifier] = mqttSettings.deviceId;
      doc[mqttSettings.wm_mqtt_client_id_identifier] = mqttSettings.clientId;
      doc[mqttSettings.wm_mqtt_hostname_identifier] = mqttSettings.hostname;
      doc[mqttSettings.wm_mqtt_port_identifier] = mqttSettings.port;
      doc[mqttSettings.wm_mqtt_user_identifier] = mqttSettings.user;
      doc[mqttSettings.wm_mqtt_password_identifier] = mqttSettings.password;
      doc[mqttSettings.wm_mqtt_basetopic_identifier] = mqttSettings.baseTopic;

      if (serializeJson(doc, configFile) == 0) {
        DEBUG_PRINTLN("[FAILED]");
      } else {
        DEBUG_PRINTLN("[DONE]");
        serializeJson(doc, Serial);
        DEBUG_PRINTLN();
      }
    }
    configFile.close();
    shouldSaveConfig = false;
  }

  //callback notifying us of the need to save config
  void saveConfigCallback() {
    saveConfig();
  }

  void initializeWifiManager() {
#ifdef ARDUINO_M5STACK_ATOMS3  // Define the M5Stack LED
    leds[0] = CRGB::Blue;      // Turn the Blue LED On
    FastLED.setBrightness(255);
    FastLED.show();
#endif
#ifdef ESP8266                         // Define the Witty ESP8266 Ports
    digitalWrite(Blue_RGB_LED, HIGH);  // Set the Blue LED full brightness
#endif
    DEBUG_PRINTLN("Starting WiFi Manager");
    // Reset Wifi settings for testing
    //wifiManager.resetSettings();

    wifiManager.setTitle("Ecodan Bridge and Proxy");

    // Set or Update the values
    custom_device_id.setValue(mqttSettings.deviceId, deviceId_max_length);
    custom_mqtt_client_id.setValue(mqttSettings.clientId, clientId_max_length);
    custom_mqtt_server.setValue(mqttSettings.hostname, hostname_max_length);
    custom_mqtt_port.setValue(mqttSettings.port, port_max_length);
    custom_mqtt_user.setValue(mqttSettings.user, user_max_length);
    custom_mqtt_pass.setValue(mqttSettings.password, password_max_length);
    custom_mqtt_basetopic.setValue(mqttSettings.baseTopic, basetopic_max_length);

    // Add the custom MQTT parameters here
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_user);
    wifiManager.addParameter(&custom_mqtt_pass);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_mqtt_basetopic);
    wifiManager.addParameter(&custom_mqtt_client_id);
    wifiManager.addParameter(&custom_device_id);

    //set minimum quality of signal so it ignores AP's under that quality
    //defaults to 8%
    //wifiManager.setMinimumSignalQuality();

    snprintf(WiFiHostname, 26, "%s%s", ClientPrefix, mqttSettings.deviceId);
    WiFi.hostname(WiFiHostname);

    wifiManager.setBreakAfterConfig(true);                  // Saves settings, even if WiFi Fails
    wifiManager.setSaveConfigCallback(saveConfigCallback);  // Set config save notify callback
    wifiManager.setAPClientCheck(true);                     // avoid timeout if client connected to softap


#ifdef ARDUINO_WT32_ETH01
    wifiManager.setConfigPortalBlocking(false);         // Doesn't block Rest of Code from operating
    if (wifiManager.autoConnect("Ecodan Bridge AP")) {  // Try connecting to WiFi
      DEBUG_PRINTLN("WiFi Connected!");
    }
#endif


#ifndef ARDUINO_WT32_ETH01
    wifiManager.setConfigPortalTimeout(120);  // Timeout before launching the config portal
    if (!wifiManager.autoConnect("Ecodan Bridge AP")) {
      DEBUG_PRINTLN("Failed to connect and hit timeout");
      delay(3000);
#ifdef ESP8266
      ESP.reset();
#endif
#ifdef ESP32
      ESP.restart();
#endif
    }
    DEBUG_PRINTLN("WiFi Connected!");
#endif

#ifdef ESP8266                        // Define the Witty ESP8266 Ports
    digitalWrite(Blue_RGB_LED, LOW);  // Set the Blue LED off, ESP32 will do this in main loop
#endif
    wifiManager.startWebPortal();
  }


  void PublishDiscoveryTopics(void) {

    // Compile Topics
    String MQTT_DISCOVERY_TOPIC, Buffer_Topic;

// -- Entities Configuration JSON -- //
#ifdef ESP8266
    String ChipModel = "ESP8266";
#endif
#ifdef ESP32
    String ChipModel = ESP.getChipModel();
#endif

    String ChipID = mqttSettings.deviceId;

    // JSON Formation
    JsonDocument Config;
    char Buffer_Payload[2048];

    // Publish all the discovery topics
    for (int i = 0; i < discovery_topics; i++) {

      if (i == 0) {  // If the first topic
        Config["device"]["identifiers"] = WiFiHostname;
        Config["device"]["manufacturer"] = "F1p";
        Config["device"]["model"] = ChipModel;
        Config["device"]["serial_number"] = ChipID;
        Config["device"]["name"] = "Ecodan ASHP";
        Config["device"]["configuration_url"] = "http://" + WiFi.localIP().toString() + ":80";
        Config["device"]["sw_version"] = FirmwareVersion;
      } else {  // Otherwise post just identifier
        Config["device"]["identifiers"] = WiFiHostname;
      }

      // Every one has a unique_id and name
      Config["unique_id"] = String(MQTT_SENSOR_UNIQUE_ID[i]) + ChipID;
      Config["name"] = String(MQTT_SENSOR_NAME[i]);


      // Sensors
      if (i >= 0 && i < 77) {
        Config["state_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[MQTT_TOPIC_POS[i]]);                               // Needs a positioner
        if (MQTT_UNITS_POS[i] > 0) { Config["unit_of_measurement"] = String(MQTT_SENSOR_UNITS[MQTT_UNITS_POS[i]]); }  // Don't send nothing
        Config["value_template"] = String(MQTT_SENSOR_VALUE_TEMPLATE[i]);
        Config["icon"] = String(MQTT_MDI_ICONS[i]);
        if (i >= 41 && i < 49) {
          Config["device_class"] = String(MQTT_DEVICE_CLASS[0]);
        }

        MQTT_DISCOVERY_TOPIC = String(MQTT_DISCOVERY_TOPICS[0]);
      }

      // Climate
      if (i >= 77 && i < 82) {
        Config["object_id"] = String(MQTT_OBJECT_ID[i - 77]);
        if (i >= 77 && i < 80) {
          Config["current_temperature_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 73]);
          Config["current_temperature_template"] = String(MQTT_SENSOR_VALUE_TEMPLATE[25]);
          Config["temperature_command_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 67]);
          Config["temperature_state_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 73]);
          Config["temperature_state_template"] = String(MQTT_SENSOR_VALUE_TEMPLATE[78]);
        } else if (i >= 80 && i < 82) {
          Config["current_temperature_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[2]);
          Config["current_temperature_template"] = String(MQTT_SENSOR_VALUE_TEMPLATE[6]);
          Config["temperature_command_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 57]);
          Config["temperature_state_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 75]);
          Config["temperature_state_template"] = String(MQTT_SENSOR_VALUE_TEMPLATE[79]);
        }
        Config["temperature_unit"] = String(MQTT_SENSOR_UNITS[9]);
        Config["max_temp"] = MQTT_CLIMATE_MAX[i - 77];
        Config["min_temp"] = MQTT_CLIMATE_MIN[i - 77];
        Config["temp_step"] = MQTT_CLIMATE_TEMP_STEP[i - 77];
        Config["precision"] = MQTT_CLIMATE_PRECISION[i - 77];
        Config["initial"] = MQTT_CLIMATE_INITAL[i - 77];
        Config["action_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[2]);
        Config["action_template"] = String(MQTT_CLIMATE_MODE_STATE_TEMPLATE[i - 77]);
        Config["mode_state_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[8]);
        Config["mode_state_template"] = String(MQTT_CLIMATE_STATE_TOPIC[i - 77]);
        if (i == 77) {
          Config["modes"][0] = "heat";
          Config["modes"][1] = "off";
        } else {
          Config["modes"][0] = "heat";
          Config["modes"][1] = "cool";
          Config["modes"][2] = "off";
          Config["mode_command_template"] = String(MQTT_CLIMATE_MODE[0]);
          Config["mode_command_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[9]);
        }

        MQTT_DISCOVERY_TOPIC = String(MQTT_DISCOVERY_TOPICS[1]);
      }

      // Switches
      if (i >= 82 && i < 92) {
        Config["state_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[MQTT_SWITCH_STATE_POS[i - 82]]);
        Config["value_template"] = String(MQTT_SENSOR_VALUE_TEMPLATE[i - 2]);
        Config["command_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 69]);
        if (i == 83) {
          Config["state_on"] = "On";
          Config["state_off"] = "Standby";
          Config["payload_on"] = "On";
          Config["payload_off"] = "Standby";
        } else {
          Config["state_on"] = ITEM_ON;
          Config["state_off"] = ITEM_OFF;
          Config["payload_on"] = ITEM_ON;
          Config["payload_off"] = ITEM_OFF;
        }
        Config["icon"] = String(MQTT_MDI_ICONS[i]);

        MQTT_DISCOVERY_TOPIC = String(MQTT_DISCOVERY_TOPICS[2]);
      }


      // Selects
      if (i >= 92 && i < 95) {
        Config["name"] = String(MQTT_SENSOR_NAME[i]);
        Config["command_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 67]);
        Config["state_topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 88]);
        Config["value_template"] = String(MQTT_SELECT_VALUE_TOPIC[i - 92]);
        if (i == 92) {
          Config["options"][0] = HotWaterControlModeString[0];
          Config["options"][1] = HotWaterControlModeString[1];
        } else {
          Config["options"][0] = "Heating Temperature";
          Config["options"][1] = "Heating Flow";
          Config["options"][2] = "Heating Compensation";
          Config["options"][3] = "Cooling Temperature";
          Config["options"][4] = "Cooling Flow";
          Config["options"][5] = "Dry Up";
        }

        MQTT_DISCOVERY_TOPIC = String(MQTT_DISCOVERY_TOPICS[4]);
      }


      // Add Availability Topics
      if (i >= 78) {
        if (i >= 86 && i < 91) {  // Server Control Mode Interlocks
          Config["availability"]["topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[8]);
          Config["availability"]["value_template"] = String(MQTT_SENSOR_VALUE_TEMPLATE[83]);
          Config["availability"]["payload_available"] = ITEM_ON;
          Config["availability"]["payload_not_available"] = ITEM_OFF;
        } else if (i >= 80 && i < 82) {  // Flow Op Mode Interlocks on Climate & Number
          Config["availability"]["topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[i - 75]);
          Config["availability"]["value_template"] = String(MQTT_NUMBER_AVAIL_TEMPLATE[0]);
        } else {  // Everything else LWT
          Config["availability"]["topic"] = MQTT_BASETOPIC + String(MQTT_TOPIC[0]);
        }
      }

      serializeJson(Config, Buffer_Payload);
      Buffer_Topic = MQTT_DISCOVERY_TOPIC + ChipID + String(MQTT_DISCOVERY_OBJ_ID[i]) + String(MQTT_DISCOVERY_TOPICS[5]);
      MQTTClient.publish(Buffer_Topic.c_str(), Buffer_Payload, true);

      MQTT_DISCOVERY_TOPIC = "";  // Clear everything ready for next loop to save RAM
      Buffer_Topic = "";
      Config.clear();
    }

    // Generate Publish Message
    DEBUG_PRINTLN("Published Discovery Topics!");
  }

  void initializeMqttClient() {
    DEBUG_PRINT("Attempting MQTT connection to: ");
    DEBUG_PRINT(mqttSettings.hostname);
    DEBUG_PRINT(":");
    DEBUG_PRINTLN(mqttSettings.port);
    MQTTClient.setServer(mqttSettings.hostname, atoi(mqttSettings.port));
  }



  void MQTTonConnect(void) {
    DEBUG_PRINTLN("MQTT ON CONNECT");
    MQTTClient.publish(MQTT_LWT.c_str(), "online");
    delay(10);

    MQTTClient.subscribe(MQTTCommandZone1FlowSetpoint.c_str());
    MQTTClient.subscribe(MQTTCommandZone1NoModeSetpoint.c_str());
    MQTTClient.subscribe(MQTTCommandZone1ProhibitHeating.c_str());
    MQTTClient.subscribe(MQTTCommandZone1ProhibitCooling.c_str());
    MQTTClient.subscribe(MQTTCommandZone1HeatingMode.c_str());
    MQTTClient.subscribe(MQTTCommandZone2FlowSetpoint.c_str());
    MQTTClient.subscribe(MQTTCommandZone2NoModeSetpoint.c_str());
    MQTTClient.subscribe(MQTTCommandZone2ProhibitHeating.c_str());
    MQTTClient.subscribe(MQTTCommandZone2ProhibitCooling.c_str());
    MQTTClient.subscribe(MQTTCommandZone2HeatingMode.c_str());
    MQTTClient.subscribe(MQTTCommandSystemHolidayMode.c_str());
    MQTTClient.subscribe(MQTTCommandHotwaterMode.c_str());
    MQTTClient.subscribe(MQTTCommandHotwaterSetpoint.c_str());
    MQTTClient.subscribe(MQTTCommandHotwaterBoost.c_str());
    MQTTClient.subscribe(MQTTCommandHotwaterNormalBoost.c_str());
    MQTTClient.subscribe(MQTTCommandHotwaterProhibit.c_str());
    MQTTClient.subscribe(MQTTCommandSystemPower.c_str());
    MQTTClient.subscribe(MQTTCommandSystemSvrMode.c_str());

    delay(10);
    PublishDiscoveryTopics();
#ifdef ARDUINO_M5STACK_ATOMS3  // Define the M5Stack LED
    leds[0] = CRGB::Green;
    FastLED.setBrightness(100);  // LED on, reduced brightness
    FastLED.show();
#endif
  }


  uint8_t MQTTReconnect() {
    if (MQTTClient.connected()) {
      return 1;
    }

    DEBUG_PRINT("With Client ID: ");
    DEBUG_PRINT(mqttSettings.clientId);
    DEBUG_PRINT(", Username: ");
    DEBUG_PRINT(mqttSettings.user);
    DEBUG_PRINT(" and Password: ");
    DEBUG_PRINTLN(mqttSettings.password);

    if (MQTTClient.connect(mqttSettings.clientId, mqttSettings.user, mqttSettings.password, MQTT_LWT.c_str(), 0, true, "offline")) {
      DEBUG_PRINTLN("MQTT Server Connected");
      MQTTonConnect();
#ifdef ESP8266                            // Define the Witty ESP8266 Ports
      digitalWrite(Red_RGB_LED, LOW);     // Turn off the Red LED
      digitalWrite(Green_RGB_LED, HIGH);  // Flash the Green LED
      delay(10);
      digitalWrite(Green_RGB_LED, LOW);
#endif
      return 1;
    } else {
#ifdef ARDUINO_M5STACK_ATOMS3  // Define the M5Stack LED
      leds[0] = CRGB::Orange;
      FastLED.setBrightness(255);  // LED on, reduced brightness
#endif
      DEBUG_PRINT("Failed with Error Code: ");
      DEBUG_PRINTLN(MQTTClient.state());
      switch (MQTTClient.state()) {
        case -4:
          DEBUG_PRINTLN("MQTT_CONNECTION_TIMEOUT");
          break;
        case -3:
          DEBUG_PRINTLN("MQTT_CONNECTION_LOST");
          break;
        case -2:
          DEBUG_PRINTLN("MQTT_CONNECT_FAILED");
          break;
        case -1:
          DEBUG_PRINTLN("MQTT_DISCONNECTED");
          break;
        case 0:
          DEBUG_PRINTLN("MQTT_CONNECTED");
          break;
        case 1:
          DEBUG_PRINTLN("MQTT_CONNECT_BAD_PROTOCOL");
          break;
        case 2:
          DEBUG_PRINTLN("MQTT_CONNECT_BAD_CLIENT_ID");
          break;
        case 3:
          DEBUG_PRINTLN("MQTT_CONNECT_UNAVAILABLE");
          break;
        case 4:
          DEBUG_PRINTLN("MQTT_CONNECT_BAD_CREDENTIALS");
          break;
        case 5:
          DEBUG_PRINTLN("MQTT_CONNECT_UNAUTHORIZED");
          break;
      }
      return 0;
    }
    return 0;
  }


  void handleMqttState() {
    if (!MQTTClient.connected()) {
#ifdef ARDUINO_M5STACK_ATOMS3  // Define the M5Stack LED
      leds[0] = CRGB::Orange;  // Turn the LED Orange
      FastLED.show();
#endif
#ifdef ESP8266                          // Define the Witty ESP8266 Ports
      analogWrite(Green_RGB_LED, 30);   // Green LED on, 25% brightness
      digitalWrite(Red_RGB_LED, HIGH);  // Add the Red LED to the Green LED = Orange
#endif
      MQTTReconnect();
    }
  }



#endif