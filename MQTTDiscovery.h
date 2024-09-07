//-- MQTT Home Assistant Auto Discovery --//

// Build the sensor JSON structure
const char MQTT_DISCOVERY_OBJ_ID[][3] PROGMEM = { "aa", "ab", "ac", "ad", "ae", "af", "ag", "ah", "ai", "aj", "ak", "al", "am", "an", "ao", "ap", "aq", "ar", "as", "at", "au", "av", "aw", "ax", "ay", "az", "ba", "bb", "bc", "bd", "be", "bf", "bg", "bh", "bi", "bj", "bk", "bl", "bm", "bn", "bo", "bp", "bq", "br", "bs", "bt", "bu", "bv", "bw", "bx", "by", "bz", "ca", "cb", "cc", "cd", "cu", "cv", "ce", "cf", "cg", "ch", "ci", "cj", "ck", "cl", "cm", "cn", "co", "cp", "cq", "cr", "cs", "ct" };

const char MQTT_SENSOR_UNIQUE_ID[][40] PROGMEM = {
  "ashp_bridge_lwt_",
  "ashp_bridge_firmware_",
  "ashp_bridge_rssi_",
  "ashp_ftc_",
  "ashp_op_mode_",
  "ashp_heater_return_",  //5
  "ashp_heater_flow_",
  "ashp_heater_setpoint_",
  "ashp_outside_air_temp_",
  "ashp_defrost_status_",
  "ashp_pump_power_",  //10
  "ashp_compressor_freq_",
  "ashp_flow_rate_",
  "ashp_run_hours_",
  "ashp_max_flow_temp_",
  "ashp_min_flow_temp_",  //15
  "ashp_boiler_flow_temp_",
  "ashp_boiler_return_temp_",
  "ashp_mixing_tank_temp_",
  "ashp_ext_flow_temp_",
  "ashp_refrige_temp_",  //20
  "ashp_immersion_active_",
  "ashp_booster_active_",
  "ashp_primary_water_pump_active_",
  "ashp_water_pump_2_active_",
  "ashp_3WayValve_1_active_",  //25
  "ashp_3WayValve_2_active_",
  "ashp_dhw_temp_",
  "ashp_dhw_temp_thw5a_",
  "ashp_dhw_prohibit_",
  "ashp_dhw_active_",  //30
  "ashp_dhw_mode_",
  "ashp_legionella_setpoint_",
  "ashp_dhw_max_tempdrop_",
  "ashp_dhw_phase_",
  "ashp_z1_flow_temp_",  //35
  "ashp_z1_return_temp_",
  "ashp_z2_flow_temp_",
  "ashp_z2_return_temp_",
  "ashp_twozone_z1_working_",
  "ashp_twozone_z2_working_",  //40
  "ashp_consumed_heat_",
  "ashp_delivered_heat_",
  "ashp_consumed_cool_",
  "ashp_delivered_cool_",
  "ashp_consumed_dhw_",  //45
  "ashp_delivered_dhw_",
  "ashp_total_consumed_",
  "ashp_total_delivered_",
  "ashp_heat_cop_",
  "ashp_cool_cop_",  //50
  "ashp_dhw_cop_",
  "ashp_total_cop_",
  "ashp_refrige_flt_code_",
  "ashp_err_code_",
  "ashp_flt_code_",  //55
  "ashp_delta_t_",
  "ashp_est_pwr_out_",

  "ashp_dhw_climate_",  //56
  "ashp_Zone1_climate_",
  "ashp_Zone2_climate_",  //58

  "ashp_dhw_boost_",    //59
  "ashp_systempower_",  //60
  "ashp_holidaymode_",
  "ashp_svr_control_mode_",
  "ashp_svrctrol_dhw_",
  "ashp_svrctrol_z1h_",
  "ashp_svrctrol_z1c_",  //65
  "ashp_svrctrol_z2h_",
  "ashp_svrctrol_z2c_",

  "ashp_z1_flow_setpoint_",  //68
  "ashp_z2_flow_setpoint_",  //69

  "ashp_dhw_mode",  //70
  "ashp_heat_cool_mode"
};


const char MQTT_MDI_ICONS[][40] PROGMEM = {
  "mdi:cloud-check-variant",
  "mdi:alpha-v-box",
  "mdi:signal-variant",
  "mdi:alpha-v-circle",
  "mdi:auto-mode",
  "mdi:keyboard-return",  //5
  "mdi:waves-arrow-right",
  "mdi:thermometer",
  "mdi:sun-thermometer-outline",
  "mdi:car-defrost-front",
  "mdi:heat-pump",  //10
  "mdi:sine-wave",
  "mdi:waves-arrow-right",
  "mdi:clock-digital",
  "mdi:thermometer-chevron-up",
  "mdi:thermometer-chevron-down",  //15
  "mdi:water-boiler",
  "mdi:water-boiler",
  "mdi:storage-tank",
  "mdi:coolant-temperature",
  "mdi:coolant-temperature",  //20
  "mdi:water-pump",
  "mdi:thermometer-plus",
  "mdi:heat-pump-outline",
  "mdi:heat-pump-outline",
  "mdi:valve",  //25
  "mdi:valve",
  "mdi:hand-water",
  "mdi:hand-water",
  "mdi:cancel",
  "mdi:water-thermometer",  //30
  "mdi:auto-mode",
  "mdi:thermometer-auto",
  "mdi:thermometer-auto",
  "mdi:auto-mode",
  "mdi:heating-coil",  //35
  "mdi:heating-coil",
  "mdi:heating-coil",
  "mdi:heating-coil",
  "mdi:heating-coil",
  "mdi:heating-coil",  //40
  "mdi:transmission-tower-import",
  "mdi:radiator",
  "mdi:transmission-tower-import",
  "mdi:radiator",
  "mdi:transmission-tower-import",  //45
  "mdi:radiator",
  "mdi:transmission-tower-import",
  "mdi:radiator",
  "mdi:calculator",
  "mdi:calculator",  //50
  "mdi:calculator",
  "mdi:calculator",
  "mdi:fridge-industrial-alert",
  "mdi:home-alert",
  "mdi:home-alert",  //55
  "mdi:delta",
  "mdi:export",  //57

  "mdi:thermostat",  //58
  "mdi:thermostat",
  "mdi:thermostat",  //60

  "mdi:thermometer-plus",  //61
  "mdi:power",
  "mdi:bag-suitcase",
  "mdi:server-network-outline",
  "mdi:cancel",             //65
  "mdi:cancel",
  "mdi:cancel",
  "mdi:cancel",
  "mdi:cancel",

  "mdi:ray-vertex",  //70
  "mdi:ray-vertex",  //71

  "mdi:auto-mode",  //72
  "mdi:sun-snowflake-variant"
};


const char MQTT_SENSOR_NAME[][40] PROGMEM = {
  "Bridge Status",
  "Firmware Version",
  "Bridge WiFi Signal",
  "FTC Version",
  "System Operation Mode",
  "Heater Return",  //5
  "Heater Flow",
  "Heater Setpoint",
  "Outside Temperature",
  "Defrost",
  "Heat Pump Output Power",  //10
  "Compressor Frequency",
  "Flow Rate",
  "Run Hours",
  "Max Flow Temperature",
  "Min Flow Temperature",  //15
  "Boiler Flow Temperature",
  "Boiler Return Temperature",
  "Mixing Tank Temperature",
  "External Flow Temperature",
  "Refrigerant Temperature",  //20
  "Immersion Heater",
  "Booster Heater",
  "Primary Water Pump",
  "Water Pump 2",
  "Three Way Valve 1",  //25
  "Three Way Valve 2",
  "DHW Temperature",
  "DHW Temperature Upper",
  "Prohibit DHW",
  "DHW Running",  //30
  "DHW Control Mode",
  "Legionella Setpoint",
  "DHW Max Temperature Drop",
  "DHW Heating Phase",
  "Zone 1 Flow Temperature",  //35
  "Zone 1 Return Temperature",
  "Zone 2 Flow Temperature",
  "Zone 2 Return Temperature",
  "Zone 1 Working",
  "Zone 2 Working",  //40
  "Consumed Heating Yesterday",
  "Delivered Heating Yesterday",
  "Consumed Cooling Yesterday",
  "Delivered Cooling Yesterday",
  "Consumed DHW Yesterday",  //45
  "Delivered DHW Yesterday",
  "Total Consumed Energy Yesterday",
  "Total Delivered Energy Yesterday",
  "Heating CoP Yesterday",
  "Cooling CoP Yesterday",  //50
  "DHW CoP Yesterday",
  "Total CoP Yesterday",
  "Refrigerant Fault Code",
  "Error Code",
  "Fault Code",  //55
  "Flow Return DeltaT",
  "Computed Output Power",

  "DHW Thermostat",  //58
  "Zone 1 Thermostat",
  "Zone 2 Thermostat",  //60

  "DHW Boost",  //61
  "System Power",
  "Holiday Mode",
  "Server Control Mode",
  "Prohibit DHW",
  "Prohibit Zone 1 Heating",
  "Prohibit Zone 1 Cooling",
  "Prohibit Zone 2 Heating",
  "Prohibit Zone 2 Cooling",  //69

  "Zone 1 Flow Setpoint",
  "Zone 2 Flow Setpoint",  //71

  "DHW Mode",  //72
  "Heating/Cooling Operation Mode"
};

const char MQTT_TOPIC[][34] PROGMEM = {
  "/LWT",                               //0
  "/Status/WiFiStatus",                 //1
  "/Status/System",                     //2
  "/Status/Advanced",                   //3
  "/Status/HotWater",                   //4
  "/Status/Zone1",                      //5
  "/Status/Zone2",                      //6
  "/Status/Energy",                     //7
  "/Status/AdvancedTwo",                //8
  "/Command/System/HeatingMode",        //9
  "/Command/HotWater/Setpoint",         //10
  "/Command/Zone1/ThermostatSetpoint",  //11
  "/Command/Zone2/ThermostatSetpoint",  //12
  "/Command/HotWater/Boost",            //13
  "/Command/System/Power",              //14
  "/Command/System/HolidayMode",        //15
  "/Command/System/SvrControlMode",     //16
  "/Command/HotWater/Prohibit",         //17
  "/Command/Zone1/ProhibitHeating",     //18
  "/Command/Zone1/ProhibitCooling",     //19
  "/Command/Zone2/ProhibitHeating",     //20
  "/Command/Zone2/ProhibitCooling",     //21
  "/Command/Zone1/FlowSetpoint",        //22
  "/Command/Zone2/FlowSetpoint",        //23
  "/Command/HotWater/Mode",             //24
  "/Command/System/HeatingMode"         //25
};


int MQTT_SWITCH_STATE_POS[] PROGMEM = {
  4,
  2,
  2,
  8,
  4,
  5,
  5,
  6,
  6
};

int MQTT_TOPIC_POS[] PROGMEM = {
  0,
  1,
  1,
  1,
  2,
  2,  //5
  2,
  2,
  2,
  2,
  2,  //10
  2,
  2,
  2,
  3,
  3,  //15
  3,
  3,
  3,
  3,
  3,  //20
  3,
  3,
  3,
  8,
  3,  //25
  8,
  4,
  4,
  4,
  4,  //30
  4,
  4,
  4,
  4,
  5,  //35
  5,
  6,
  6,
  5,
  6,  //40
  7,
  7,
  7,
  7,
  7,  //45
  7,
  7,
  7,
  7,
  7,  //50
  7,
  7,
  8,
  8,
  8,  //55
  2,
  2
};

int MQTT_UNITS_POS[] PROGMEM = {
  0,
  0,
  1,
  0,
  0,
  2,  //5
  2,
  2,
  2,
  0,
  3,  //10
  4,
  5,
  6,
  2,
  2,  //15
  2,
  2,
  2,
  2,
  2,  //20
  0,
  0,
  0,
  0,
  0,  //25
  0,
  2,
  2,
  0,
  0,  //30
  0,
  2,
  2,
  0,
  2,  //35
  2,
  2,
  2,
  0,
  0,  //40
  7,
  7,
  7,
  7,
  7,  //45
  7,
  7,
  7,
  8,
  8,  //50
  8,
  8,
  0,
  0,
  0,  //55
  2,
  3
};

const char MQTT_OBJECT_ID[][14] PROGMEM = {
  "dhw_climate",
  "zone1_climate",
  "zone2_climate"
};

int MQTT_CLIMATE_MAX[] PROGMEM = {
  60,
  30,
  30
};

int MQTT_CLIMATE_MIN[] PROGMEM = {
  0,
  10,
  10
};

float MQTT_CLIMATE_TEMP_STEP[] PROGMEM = {
  1.0,
  0.5,
  0.5
};

float MQTT_CLIMATE_PRECISION[] PROGMEM = {
  0.5,
  0.5,
  0.5
};

int MQTT_CLIMATE_INITAL[] PROGMEM = {
  50,
  10,
  10
};

const char MQTT_CLIMATE_MODE[][127] PROGMEM = {
  "{% set values = { 'heat':'Heating Temperature', 'cool':'Cooling Temperature'} %} {{ values[value] if value in values.keys() }}"
};

const char MQTT_CLIMATE_STATE_TOPIC[][494] PROGMEM = {
  "{{ 'heat' if (value_json.SystemOperationMode=='Hot Water' or value_json.SystemOperationMode=='Legionella') else 'off' }}",
  "{{ 'heat' if (value_json.SystemOperationMode=='Heating' and state_attr('climate.zone2_climate','current_temperature')=='0') else 'heat' if (value_json.SystemOperationMode == 'Heating' and states('sensor.ecodan_ashp_zone_1_working')=='1') else 'cool' if (value_json.SystemOperationMode == 'Cooling' and state_attr('climate.zone2_climate','current_temperature')=='0') else 'cool' if (value_json.SystemOperationMode == 'Cooling' and states('sensor.ecodan_ashp_zone_1_working')=='1') else 'off' }}",
  "{{ 'heat' if (value_json.SystemOperationMode=='Heating' and state_attr('climate.zone2_climate','current_temperature')=='0') else 'heat' if (value_json.SystemOperationMode == 'Heating' and states('sensor.ecodan_ashp_zone_2_working')=='1') else 'cool' if (value_json.SystemOperationMode == 'Cooling' and state_attr('climate.zone2_climate','current_temperature')=='0') else 'cool' if (value_json.SystemOperationMode == 'Cooling' and states('sensor.ecodan_ashp_zone_2_working')=='1') else 'off' }}"
};

const char MQTT_SELECT_VALUE_TOPIC[][405] PROGMEM = {
  "{{ 'Normal' if value_json.HotWaterControlMode=='Normal' else'Eco' if value_json.HotWaterControlMode=='Eco' }}",
  "{{ 'Heating Temperature' if value_json.HeatingControlMode=='Temp' else 'Heating Flow' if value_json.HeatingControlMode=='Flow' else 'Heating Compensation' if value_json.HeatingControlMode == 'Compensation' else 'Cooling Temperature' if value_json.HeatingControlMode == 'Cool' else 'Cooling Flow' if value_json.HeatingControlMode == 'Cool Flow' else 'Dry Up' if value_json.HeatingControlMode == 'Dry Up'}}"
};

const char MQTT_SENSOR_UNITS[][6] PROGMEM = {
  "",
  "dBm",
  "°C",
  "kW",
  "Hz",
  "l/min",
  "hrs",
  "kWh",
  "CoP",
  "C"
};

const char MQTT_SENSOR_VALUE_TEMPLATE[][50] PROGMEM = {
  "{{ value if value is defined else 'Unknown' }}",
  "{{ value_json.Firmware }}",
  "{{ value_json.RSSI }}",
  "{{ value_json.FTCVersion }}",
  "{{ value_json.SystemOperationMode }}",
  "{{ value_json.HeaterReturn }}",  //5
  "{{ value_json.HeaterFlow }}",
  "{{ value_json.HeaterSetpoint }}",
  "{{ value_json.OutsideTemp }}",
  "{{ value_json.Defrost }}",
  "{{ value_json.HeaterPower }}",  //10
  "{{ value_json.Compressor }}",
  "{{ value_json.FlowRate }}",
  "{{ value_json.RunHours }}",
  "{{ value_json.FlowTMax }}",
  "{{ value_json.FlowTMin }}",  //15
  "{{ value_json.BoilerFlow }}",
  "{{ value_json.BoilerReturn }}",
  "{{ value_json.MixingTemp }}",
  "{{ value_json.ExternalFlowTemp }}",
  "{{ value_json.RefrigeTemp }}",  //20
  "{{ value_json.Immersion }}",
  "{{ value_json.Booster }}",
  "{{ value_json.PrimaryWaterPump }}",
  "{{ value_json.WaterPump2 }}",
  "{{ value_json.ThreeWayValve }}",  //25
  "{{ value_json.ThreeWayValve2 }}",
  "{{ value_json.Temperature }}",
  "{{ value_json.TempTHW5A }}",
  "{{ value_json.ProhibitDHW }}",
  "{{ value_json.DHWActive }}",  //30
  "{{ value_json.HotWaterControlMode }}",
  "{{ value_json.LegionellaSetpoint }}",
  "{{ value_json.HotWaterMaxTDrop }}",
  "{{ value_json.HotWaterPhase }}",
  "{{ value_json.FlowTemp }}",
  "{{ value_json.ReturnTemp }}",
  "{{ value_json.FlowTemp }}",
  "{{ value_json.ReturnTemp }}",
  "{{ value_json.TwoZone_Z1Working }}",
  "{{ value_json.TwoZone_Z2Working }}",  // 40
  "{{ value_json.CHEAT|round(2) }}",
  "{{ value_json.DHEAT|round(2) }}",
  "{{ value_json.CCOOL|round(2) }}",
  "{{ value_json.DCOOL|round(2) }}",
  "{{ value_json.CDHW|round(2) }}",  //45
  "{{ value_json.DDHW|round(2) }}",
  "{{ value_json.CTOTAL|round(2) }}",
  "{{ value_json.DTOTAL|round(2) }}",
  "{{ value_json.HEAT_CoP|round(2) }}",
  "{{ value_json.COOL_CoP|round(2) }}",  //50
  "{{ value_json.DHW_CoP|round(2) }}",
  "{{ value_json.TOTAL_CoP|round(2) }}",
  "{{ value_json.RefrigeFltCode }}",
  "{{ value_json.ErrCode }}",
  "{{ value_json.FltCode }}",  //55
  "{{ value_json.FlowReturnDeltaT }}",
  "{{ value_json.EstOutputPower }}",
  "{{ value_json }}",
  "{{ value_json.Setpoint }}",
  "{{ value_json.FSP }}",             //60
  "{{ value_json.HotWaterBoostActive }}",
  "{{ value_json.SystemPower }}",
  "{{ value_json.HolidayMode }}",
  "{{ value_json.SvrControlMode }}",
  "{{ value_json.ProhibitDHW }}",       //65
  "{{ value_json.ProhibitHeating }}",
  "{{ value_json.ProhibitCooling }}",
  "{{ value_json.ProhibitHeating }}",
  "{{ value_json.ProhibitCooling }}"    //69
};

const char MQTT_DISCOVERY_TOPICS[][23] PROGMEM = {
  "homeassistant/sensor/",
  "homeassistant/climate/",
  "homeassistant/switch/",
  "homeassistant/number/",
  "homeassistant/select/",
  "/config"
};

const char MQTT_ENERGY_CLASS[][21] PROGMEM = {
  "energy"
};