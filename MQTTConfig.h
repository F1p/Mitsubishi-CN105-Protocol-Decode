String MQTT_BASETOPIC = "Ecodan/ASHP";

String MQTT_LWT = MQTT_BASETOPIC + "/LWT";
String MQTT_STATUS = MQTT_BASETOPIC + "/Status";
String MQTT_COMMAND = MQTT_BASETOPIC + "/Command";

String MQTT_STATUS_ZONE1 = MQTT_STATUS + "/Zone1";
String MQTT_STATUS_ZONE2 = MQTT_STATUS + "/Zone2";
String MQTT_STATUS_HOTWATER = MQTT_STATUS + "/HotWater";
String MQTT_STATUS_SYSTEM = MQTT_STATUS + "/System";
String MQTT_STATUS_CONFIGURATION = MQTT_STATUS + "/Configuration";
String MQTT_STATUS_ADVANCED = MQTT_STATUS + "/Advanced";
String MQTT_STATUS_ADVANCED_TWO = MQTT_STATUS + "/AdvancedTwo";
String MQTT_STATUS_ENERGY = MQTT_STATUS + "/Energy";
String MQTT_STATUS_WIFISTATUS = MQTT_STATUS + "/WiFiStatus";
String MQTT_STATUS_CURVE = MQTT_STATUS + "/CompCurve";
String MQTT_STATUS_ACTV_CTRL = MQTT_STATUS + "/ActiveControl";

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
String MQTT_COMMAND_SYSTEM_UNITSIZE = MQTT_COMMAND_SYSTEM + "/UnitSize";
String MQTT_COMMAND_SYSTEM_GLYCOL = MQTT_COMMAND_SYSTEM + "/Glycol";
String MQTT_COMMAND_SYSTEM_SERVICE = MQTT_COMMAND_SYSTEM + "/Svc";
String MQTT_COMMAND_SYSTEM_COMPCURVE = MQTT_COMMAND_SYSTEM + "/CompCurve";
String MQTT_COMMAND_SYSTEM_ACTV_CTRL = MQTT_COMMAND_SYSTEM + "/ActiveControl";

String MQTTCommand = MQTT_COMMAND;
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
String MQTTCommandSystemUnitSize = MQTT_COMMAND_SYSTEM_UNITSIZE;
String MQTTCommandSystemGlycol = MQTT_COMMAND_SYSTEM_GLYCOL;
String MQTTCommandSystemService = MQTT_COMMAND_SYSTEM_SERVICE;
String MQTTCommandSystemCompCurve = MQTT_COMMAND_SYSTEM_COMPCURVE;
String MQTTCommandSystemActvCtrl = MQTT_COMMAND_SYSTEM_ACTV_CTRL;


const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
char snprintbuffer[41] = "";
char DeviceID[15] = "";
const char ClientPrefix[15] = "MelPumpBridge-";
char WiFiHostname[40] = "";



// Programs

#if defined(ESP8266) || defined(ESP32)  // ESP32 or ESP8266 Compatiability
void readSettingsFromConfig() {
  // Clean LittleFS for testing

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
            serializeJson(doc, DEBUGPORT);
            DEBUG_PRINTLN();

            // Build in safety check, otherwise ESP will crash out and you can't get back in
            if (doc.containsKey(mqttSettings.wm_device_id_identifier)) {
              if ((strlen(doc[mqttSettings.wm_device_id_identifier]) > 0) && ((strlen(doc[mqttSettings.wm_device_id_identifier]) + 1) <= deviceId_max_length)) {
                strcpy(mqttSettings.deviceId, doc[mqttSettings.wm_device_id_identifier]);
              }
            } else {
#ifdef ESP8266
              snprintf(snprintbuffer, deviceId_max_length, (String(ESP.getChipId(), HEX)).c_str());
#endif
#ifdef ESP32
              snprintf(snprintbuffer, deviceId_max_length, (String(ESP.getEfuseMac(), HEX)).c_str());
#endif
              strcpy(mqttSettings.deviceId, snprintbuffer);
              shouldSaveConfig = true;  // Save config after exit to update the file
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_hostname_identifier)) {
              if ((strlen(doc[mqttSettings.wm_mqtt_hostname_identifier]) > 0) && ((strlen(doc[mqttSettings.wm_mqtt_hostname_identifier]) + 1) <= hostname_max_length)) {
                //strcpy(mqttSettings.hostname, doc[mqttSettings.wm_mqtt_hostname_identifier]);   // Hard coded mqtt.melpump.com in this firmware
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_port_identifier)) {
              strcpy(mqttSettings.port, doc[mqttSettings.wm_mqtt_port_identifier]);
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_user_identifier)) {
              if ((strlen(doc[mqttSettings.wm_mqtt_user_identifier]) > 0) && ((strlen(doc[mqttSettings.wm_mqtt_user_identifier]) + 1) <= user_max_length)) {
                strcpy(mqttSettings.user, doc[mqttSettings.wm_mqtt_user_identifier]);
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_password_identifier)) {
              if ((strlen(doc[mqttSettings.wm_mqtt_password_identifier]) > 0) && ((strlen(doc[mqttSettings.wm_mqtt_password_identifier]) + 1) <= password_max_length)) {
                strcpy(mqttSettings.password, doc[mqttSettings.wm_mqtt_password_identifier]);
              }
            }
            if (doc.containsKey(mqttSettings.wm_mqtt_basetopic_identifier)) {
              if ((strlen(doc[mqttSettings.wm_mqtt_basetopic_identifier]) > 0) && ((strlen(doc[mqttSettings.wm_mqtt_basetopic_identifier]) + 1) <= basetopic_max_length)) {
                strcpy(mqttSettings.baseTopic, mqttSettings.deviceId);
                MQTT_BASETOPIC = mqttSettings.deviceId;  // Ignores the base topic saved in JSON as it should be always be deviceID for MELPump
              }
            } else {
              MQTT_BASETOPIC = mqttSettings.deviceId;  // Ignores the base topic saved in JSON as it should be always be deviceID for MELPump
              shouldSaveConfig = true;                 // Save config after exit to update the file
            }
            // Unit Size
            if (doc.containsKey(unitSettings.unitsize_identifier)) {
              if (doc[unitSettings.unitsize_identifier] > 0) {
                unitSettings.UnitSize = doc[unitSettings.unitsize_identifier];
              }
            } else {                    // For upgrading from <6.1.1, create the entry
              shouldSaveConfig = true;  // Save config after exit to update the file
            }
            // Glycol Strength
            if (doc.containsKey(unitSettings.glycol_identifier)) {
              if (doc[unitSettings.glycol_identifier] > 0) {
                unitSettings.GlycolStrength = doc[unitSettings.glycol_identifier];
              }
            } else {                    // For upgrading from <6.1.1, create the entry
              shouldSaveConfig = true;  // Save config after exit to update the file
            }
            // Compensation Curve
            if (doc.containsKey(unitSettings.compcurve_identifier)) {
              if (strlen(doc[unitSettings.compcurve_identifier]) > 0) {
                unitSettings.CompCurve = doc[unitSettings.compcurve_identifier].as<String>();
              }
            } else {                    // For upgrading from <6.3.0, create the entry
              shouldSaveConfig = true;  // Save config after exit to update the file
            }
            // Short Cycle Protection
            if (doc.containsKey(unitSettings.act_ctrl_sc_identifier)) {
              unitSettings.shortcycleprotectionenabled = doc[unitSettings.act_ctrl_sc_identifier].as<bool>();
            } else {                    // For upgrading from <6.4.0, create the entry
              shouldSaveConfig = true;  // Save config after exit to update the file
            }
          }
        }
        configFile.close();
      } else {
        DEBUG_PRINTLN("No config file exists, using placeholder values");
        // Populate the Dynamic Variables (Device ID)
#ifdef ESP8266
        snprintf(DeviceID, deviceId_max_length, (String(ESP.getChipId(), HEX)).c_str());
#endif
#ifdef ESP32
        snprintf(DeviceID, deviceId_max_length, (String(ESP.getEfuseMac(), HEX)).c_str());
#endif
        strcpy(mqttSettings.deviceId, DeviceID);
        strcpy(mqttSettings.baseTopic, DeviceID);

        // Create a Random Password
        String randomString;
        for (int i = 0; i < 16; ++i) {
          int index = random(sizeof(charset) - 1);
          randomString += charset[index];
        }
        strcpy(mqttSettings.password, randomString.c_str());
      }

      if (strcmp(mqttSettings.user, "") == 0) {  // First Power On, when placeholders were created
        shouldPOSTMELPump = true;
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
    MQTT_STATUS_CONFIGURATION = MQTT_STATUS + "/Configuration";
    MQTT_STATUS_ADVANCED = MQTT_STATUS + "/Advanced";
    MQTT_STATUS_ADVANCED_TWO = MQTT_STATUS + "/AdvancedTwo";
    MQTT_STATUS_ENERGY = MQTT_STATUS + "/Energy";
    MQTT_STATUS_WIFISTATUS = MQTT_STATUS + "/WiFiStatus";
    MQTT_STATUS_CURVE = MQTT_STATUS + "/CompCurve";
    MQTT_STATUS_ACTV_CTRL = MQTT_STATUS + "/ActiveControl";

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
    MQTT_COMMAND_SYSTEM_UNITSIZE = MQTT_COMMAND_SYSTEM + "/UnitSize";
    MQTT_COMMAND_SYSTEM_GLYCOL = MQTT_COMMAND_SYSTEM + "/Glycol";
    MQTT_COMMAND_SYSTEM_SERVICE = MQTT_COMMAND_SYSTEM + "/Svc";
    MQTT_COMMAND_SYSTEM_COMPCURVE = MQTT_COMMAND_SYSTEM + "/CompCurve";
    MQTT_COMMAND_SYSTEM_ACTV_CTRL = MQTT_COMMAND_SYSTEM + "/ActiveControl";

    MQTTCommand = MQTT_COMMAND;
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
    MQTTCommandSystemUnitSize = MQTT_COMMAND_SYSTEM_UNITSIZE;
    MQTTCommandSystemGlycol = MQTT_COMMAND_SYSTEM_GLYCOL;
    MQTTCommandSystemService = MQTT_COMMAND_SYSTEM_SERVICE;
    MQTTCommandSystemCompCurve = MQTT_COMMAND_SYSTEM_COMPCURVE;
    MQTTCommandSystemActvCtrl = MQTT_COMMAND_SYSTEM_ACTV_CTRL;
  }




  void saveConfig() {
    // Read MQTT Portal Values for save to file system
    DEBUG_PRINTLN("Copying Portal Values...");
    strcpy(mqttSettings.user, custom_mqtt_user.getValue());
    strcpy(mqttSettings.password, custom_mqtt_pass.getValue());

    DEBUG_PRINT("Saving config... ");
    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile) {
      DEBUG_PRINTLN("[FAILED] Unable to open config file for writing");
    } else {
      JsonDocument doc;
      doc[mqttSettings.wm_device_id_identifier] = mqttSettings.deviceId;
      doc[mqttSettings.wm_mqtt_hostname_identifier] = mqttSettings.hostname;
      doc[mqttSettings.wm_mqtt_port_identifier] = mqttSettings.port;
      doc[mqttSettings.wm_mqtt_user_identifier] = mqttSettings.user;
      doc[mqttSettings.wm_mqtt_password_identifier] = mqttSettings.password;
      doc[mqttSettings.wm_mqtt_basetopic_identifier] = mqttSettings.baseTopic;
      doc[unitSettings.unitsize_identifier] = unitSettings.UnitSize;
      doc[unitSettings.glycol_identifier] = unitSettings.GlycolStrength;
      doc[unitSettings.compcurve_identifier] = unitSettings.CompCurve;
      doc[unitSettings.act_ctrl_sc_identifier] = unitSettings.shortcycleprotectionenabled;

      if (serializeJson(doc, configFile) == 0) {
        DEBUG_PRINTLN("[FAILED]");
      } else {
        DEBUG_PRINTLN("[DONE]");
        serializeJson(doc, DEBUGPORT);
        DEBUG_PRINTLN();
#ifndef ARDUINO_WT32_ETH01
        if (WiFi.status() == WL_CONNECTED) {
          DEBUG_PRINTLN(F("Restarting Web Server..."));  // Restart the web server now it's on WiFi
          wifiManager.stopWebPortal();
          wifiManager.startWebPortal();
          MDNS.end();
          MDNS.begin("heatpump");
          MDNS.addService("http", "tcp", 80);
        }
#endif
      }
    }
    configFile.close();
    shouldSaveConfig = false;
  }

  //callback notifying us of the need to save config
  void saveConfigCallback() {
    saveConfig();
    MelPumpDebugPOST();
    shouldPOSTMELPump = true;
  }

  // Handle Webhook Callbacks
  void handleRoute() {
    if (wifiManager.server->hasArg("z1temp")) {  // Pull Argument room1
      String input = wifiManager.server->arg("z1temp");
      unitSettings.z1_room_temperature = input.toFloat();
      wifiManager.server->send(200, "text/plain", "success");
    } else if (wifiManager.server->hasArg("z1setpoint")) {
      String input = wifiManager.server->arg("z1setpoint");
      unitSettings.z1_room_setpoint = input.toFloat();
      wifiManager.server->send(200, "text/plain", "success");
    } else if (wifiManager.server->hasArg("z2temp")) {
      String input = wifiManager.server->arg("z2temp");
      unitSettings.z2_room_temperature = input.toFloat();
      wifiManager.server->send(200, "text/plain", "success");
    } else if (wifiManager.server->hasArg("z2setpoint")) {
      String input = wifiManager.server->arg("z2setpoint");
      unitSettings.z2_room_setpoint = input.toFloat();
      wifiManager.server->send(200, "text/plain", "success");
    } else {
      wifiManager.server->send(400, "text/plain", "failed");
    }
  }

  // Callback for webhooks
  void bindServerCallback() {
    wifiManager.server->on("/webhook", handleRoute);
  }

  void initializeWifiManager() {
    DEBUG_PRINTLN("Starting WiFi Manager");
    // Reset Wifi settings for testing
    wifiManager.setTitle("MelPump Ecodan Bridge");

    // Set or Update the values
    custom_device_id.setValue(mqttSettings.deviceId, deviceId_max_length);
    custom_mqtt_user.setValue(mqttSettings.user, user_max_length);
    custom_mqtt_pass.setValue(mqttSettings.password, password_max_length);

    // Add the custom MQTT parameters here
    wifiManager.addParameter(&custom_device_id);
    wifiManager.addParameter(&custom_mqtt_user);
    wifiManager.addParameter(&custom_mqtt_pass);

    //set minimum quality of signal so it ignores AP's under that quality
    //defaults to 8%
    //wifiManager.setMinimumSignalQuality();

    snprintf(WiFiHostname, 40, "%s%s", ClientPrefix, mqttSettings.deviceId);
    WiFi.hostname(WiFiHostname);
#ifdef ESP8266                         // Define the Witty ESP8266 Ports
    digitalWrite(Blue_RGB_LED, HIGH);  // Turn the Blue LED On
#endif
    wifiManager.setConfigPortalBlocking(false);             // Non-Blocking portal
    wifiManager.setBreakAfterConfig(true);                  // Saves settings, even if WiFi Fails
    wifiManager.setSaveConfigCallback(saveConfigCallback);  // Set config save callback
    wifiManager.setAPClientCheck(true);                     // Avoid timeout if client connected to softap
    wifiManager.setWebServerCallback(bindServerCallback);   // Callback for the webhook route

#ifndef ARDUINO_WT32_ETH01
    wifiManager.setConfigPortalTimeout(600);  // Timeout before launching the config portal (WiFi Only)
    wifiManager.setConnectTimeout(15);        // Set WiFi Connection Timeout
    if (!wifiManager.autoConnect("Ecodan Bridge AP")) {
      DEBUG_PRINTLN("Failed to connect and hit timeout");
    } else {
      DEBUG_PRINTLN("WiFi Connected!");
    }
#endif
  }


  void initializeMQTTClient1() {
    DEBUG_PRINT("Attempting MQTT connection to: ");
    DEBUG_PRINT(mqttSettings.hostname);
    DEBUG_PRINT(":");
    DEBUG_PRINTLN(8883);                                 // Hard Code MQTTS Port 8883 (Default stored for HA migrators is 1883)
    NetworkClient.setCACert(melpump_mqtt_root_ca);       // Only MELPump uses MQTTS and this Root CA
    MQTTClient1.setServer(mqttSettings.hostname, 8883);  // Hard Code the MQTTS Port
  }

  void MQTTonConnect(void) {
    DEBUG_PRINTLN("MQTT ON CONNECT");

    MQTTClient1.publish(MQTT_LWT.c_str(), "online");
    MQTTClient1.loop();

    // Helper lambda: Automatically retries up to maxRetries times if subscription fails
    auto safeSubscribeWithRetry = [](const String& topic, uint8_t maxRetries = 3) {
      if (topic.length() == 0) return;

      bool subscribed = false;
      uint8_t attempt = 0;

      while (!subscribed && attempt < maxRetries) {
        attempt++;
        subscribed = MQTTClient1.subscribe(topic.c_str());
        MQTTClient1.loop();  // Process broker SUBACK / network traffic

        if (subscribed) {
          DEBUG_PRINT("Subscribed: ");
          DEBUG_PRINTLN(topic);
        } else {
          DEBUG_PRINT("RETRY ");
          DEBUG_PRINT(attempt);
          DEBUG_PRINT("/");
          DEBUG_PRINT(maxRetries);
          DEBUG_PRINT(" Failed to subscribe: ");
          DEBUG_PRINTLN(topic);

          // Small yield to let network state settle if client lost connection
          delay(50);

          // Break out early if connection drops during retry attempts
          if (!MQTTClient1.connected()) break;
        }
      }
    };

    safeSubscribeWithRetry(MQTTCommand);
    safeSubscribeWithRetry(MQTTCommandZone1FlowSetpoint);
    safeSubscribeWithRetry(MQTTCommandZone1NoModeSetpoint);
    safeSubscribeWithRetry(MQTTCommandZone1ProhibitHeating);
    safeSubscribeWithRetry(MQTTCommandZone1ProhibitCooling);
    safeSubscribeWithRetry(MQTTCommandZone1HeatingMode);
    safeSubscribeWithRetry(MQTTCommandZone2FlowSetpoint);
    safeSubscribeWithRetry(MQTTCommandZone2NoModeSetpoint);
    safeSubscribeWithRetry(MQTTCommandZone2ProhibitHeating);
    safeSubscribeWithRetry(MQTTCommandZone2ProhibitCooling);
    safeSubscribeWithRetry(MQTTCommandZone2HeatingMode);
    safeSubscribeWithRetry(MQTTCommandHotwaterMode);
    safeSubscribeWithRetry(MQTTCommandHotwaterSetpoint);
    safeSubscribeWithRetry(MQTTCommandHotwaterBoost);
    safeSubscribeWithRetry(MQTTCommandHotwaterNormalBoost);
    safeSubscribeWithRetry(MQTTCommandHotwaterProhibit);
    safeSubscribeWithRetry(MQTTCommandSystemHolidayMode);
    safeSubscribeWithRetry(MQTTCommandSystemPower);
    safeSubscribeWithRetry(MQTTCommandSystemSvrMode);
    safeSubscribeWithRetry(MQTTCommandSystemUnitSize);
    safeSubscribeWithRetry(MQTTCommandSystemGlycol);
    safeSubscribeWithRetry(MQTTCommandSystemService);
    safeSubscribeWithRetry(MQTTCommandSystemCompCurve);
    safeSubscribeWithRetry(MQTTCommandSystemActvCtrl);

#ifdef ESP8266
    analogWrite(Green_RGB_LED, 30);
    digitalWrite(Red_RGB_LED, LOW);
#endif

#ifdef ARDUINO_M5STACK_ATOMS3
    myLED.setPixel(0, L_GREEN, 1);
    myLED.brightness(LED_BRIGHT, 1);
#endif
  }


  uint8_t MQTTReconnect() {
    if (MQTTClient1.connected()) {
      return 1;
    } else if (WiFi.status() == WL_CONNECTED && strcmp(mqttSettings.user, "") != 0 && strcmp(mqttSettings.password, "") != 0) {
      initializeMQTTClient1();
      DEBUG_PRINT("with Device ID: ");
      DEBUG_PRINT(mqttSettings.deviceId);
      DEBUG_PRINT(", Username: ");
      DEBUG_PRINT(mqttSettings.user);
      DEBUG_PRINT(" and Password: ");
      DEBUG_PRINTLN(mqttSettings.password);

      if (MQTTClient1.connect(mqttSettings.deviceId, mqttSettings.user, mqttSettings.password, MQTT_LWT.c_str(), 0, true, "offline")) {
        DEBUG_PRINTLN("MQTT Server Connected");
        MQTTonConnect();
#ifdef ESP8266                              // Define the Witty ESP8266 Ports
        digitalWrite(Red_RGB_LED, LOW);     // Turn off the Red LED
        digitalWrite(Green_RGB_LED, HIGH);  // Flash the Green LED
        delay(10);
        digitalWrite(Green_RGB_LED, LOW);
#endif
        return 1;
      } else {
#ifdef ARDUINO_M5STACK_ATOMS3  // Define the M5Stack LED
        //FastLED.setBrightness(255);  // LED on, reduced brightness
        if (!wifiManager.getConfigPortalActive()) {  // Not got config portal open, change to orange:
          if (WiFi.status() != WL_CONNECTED) {
            myLED.setPixel(0, L_RED, 1);  // set the LED colour and show it
          } else {
            myLED.setPixel(0, L_ORANGE, 1);  // set the LED colour and show it
          }
        }
#endif
        switch (MQTTClient1.state()) {
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
    } else {
      DEBUG_PRINTLN(F("Skipping MQTT Connection as Username/Password is empty"));
      return 0;
    }
  }


  void handleMQTTState() {
    if (!MQTTClient1.connected()) {
#ifdef ARDUINO_M5STACK_ATOMS3                      // Define the M5Stack LED
      if (!wifiManager.getConfigPortalActive()) {  // Not got config portal open, change to orange:
        if (WiFi.status() != WL_CONNECTED) {
          myLED.setPixel(0, L_RED, 1);  // set the LED colour and show it
        } else {
          myLED.setPixel(0, L_ORANGE, 1);  // set the LED colour and show it
        }
      }
#endif
#ifdef ESP8266                          // Define the Witty ESP8266 Ports
      analogWrite(Green_RGB_LED, 30);   // Green LED on, 25% brightness
      digitalWrite(Red_RGB_LED, HIGH);  // Add the Red LED to the Green LED = Orange
#endif
      MQTTReconnect();
      delay(10);
    }
  }



#endif