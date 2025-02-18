//-- MQTT Home Assistant Auto Discovery --//

const int discovery_topics PROGMEM = 108;

// Build the sensor JSON structure
const char MQTT_DISCOVERY_OBJ_ID[][3] PROGMEM = { "aa", "ab", "ac", "ad", "ae", "af", "ag", "ai", "aj", "ak", "al", "am", "an", "ao", "ap", "aq", "ar", "as", "au", "av", "aw", "ax", "ay", "az", "ba", "bb", "bc", "bd", "be", "bf", "bg", "bh", "bi", "bj", "bk", "bl", "bm", "bn", "bo", "bp", "bq", "br", "bs", "bt", "bu", "bv", "bw", "bx", "by", "bz", "ca", "cb", "cc", "cd", "cu", "cv", "cw", "cx", "cz", "da", "db", "dc", "de", "df", "dg", "dh", "di", "dj", "dk", "dl", "dm", "dn", "do", "dp", "dq", "ds", "dt", "dx", "dz", "ea", "eb", "ec", "ed", "ee", "ef", "eg", "eh", "ei", "ce", "cf", "cg", "dw", "du", "ch", "ci", "cj", "ck", "cl", "cm", "cn", "co", "cp", "dr", "cs", "ct", "dv", "dx", "dy" };

const char MQTT_SENSOR_UNIQUE_ID[][32] PROGMEM = {
  "ashp_bridge_lwt_",
  "ashp_bridge_firmware_",
  "ashp_bridge_rssi_",
  "ashp_ftc_",
  "ashp_op_mode_",
  "ashp_heater_return_",  //5
  "ashp_heater_flow_",
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
  "ashp_est_heat_pwr_out_",
  "ashp_est_cool_pwr_out_",
  "ashp_heat_active_",
  "ashp_cool_active_",  //60
  "ashp_z1h_prohibit_",
  "ashp_z1c_prohibit_",
  "ashp_z2h_prohibit_",
  "ashp_z2c_prohibit_",
  "ashp_z1_fsp",  //65
  "ashp_z2_fsp",
  "ashp_z1_tstat_demand",
  "ashp_z2_tstat_demand",
  "ashp_ext_tstat_demand",
  "ashp_water_pump_4_active",
  "ashp_water_pump_3_active",
  "ashp_water_pump_13_active",
  "ashp_input_power_",
  "ashp_est_pwr_in_",
  "ashp_ftc_sw_v_",
  "ashp_dhw_eco_boost_",
  "ashp_mix_valve_pos_",
  "ashp_bridge_cpu_",
  "ashp_booster2_active_",
  "ashp_instant_cop_",
  "ashp_liquid_temp_",
  "ashp_comp_op_times",   //eb
  "ashp_th4_discharge_",  //ec
  "ashp_subcool_",        //ed
  "ashp_th8_",            //ef
  "ashp_th6_",            //eg
  "ashp_fan1_rpm_",       //eh
  "ashp_fan2_rpm_",       //ei
  "ashp_LEVA_",           //ej


  "ashp_dhw_climate_",  //65
  "ashp_Zone1_climate_",
  "ashp_Zone2_climate_",  //67
  "ashp_Zone1_flow_climate_",
  "ashp_Zone2_flow_climate_",  //67

  "ashp_dhw_boost_",  //68
  "ashp_systempower_",
  "ashp_holidaymode_",
  "ashp_svr_control_mode_",
  "ashp_svrctrol_dhw_",
  "ashp_svrctrol_z1h_",  //73
  "ashp_svrctrol_z1c_",
  "ashp_svrctrol_z2h_",
  "ashp_svrctrol_z2c_",
  "ashp_svrctrol_norm_dhw_",

  "ashp_dhw_mode",  //79
  "ashp_heat_cool_mode",
  "ashp_heat_cool_mode_z2",
  "ashp_unit_size_",
  "ashp_glycol_",
};


const char MQTT_MDI_ICONS[][30] PROGMEM = {
  "mdi:cloud-check-variant",
  "mdi:alpha-v-box",
  "mdi:signal-variant",
  "mdi:alpha-v-circle",
  "mdi:auto-mode",
  "mdi:keyboard-return",  //5
  "mdi:waves-arrow-right",
  "mdi:sun-thermometer-outline",
  "mdi:car-defrost-front",
  "mdi:heat-pump",
  "mdi:sine-wave",  //10
  "mdi:waves-arrow-right",
  "mdi:clock-digital",
  "mdi:thermometer-chevron-up",
  "mdi:thermometer-chevron-down",
  "mdi:water-boiler",  //15
  "mdi:water-boiler",
  "mdi:storage-tank",
  "mdi:coolant-temperature",
  "mdi:water-pump",
  "mdi:thermometer-plus",  //20
  "mdi:heat-pump-outline",
  "mdi:heat-pump-outline",
  "mdi:valve",
  "mdi:valve",
  "mdi:hand-water",  //25
  "mdi:hand-water",
  "mdi:cancel",
  "mdi:water-thermometer",
  "mdi:auto-mode",
  "mdi:thermometer-auto",  //30
  "mdi:thermometer-auto",
  "mdi:auto-mode",
  "mdi:heating-coil",
  "mdi:heating-coil",
  "mdi:heating-coil",  //35
  "mdi:heating-coil",
  "mdi:heating-coil",
  "mdi:heating-coil",
  "mdi:transmission-tower-import",
  "mdi:radiator",  //40
  "mdi:transmission-tower-import",
  "mdi:radiator",
  "mdi:transmission-tower-import",
  "mdi:radiator",
  "mdi:transmission-tower-import",  //45
  "mdi:radiator",
  "mdi:calculator",
  "mdi:calculator",
  "mdi:calculator",
  "mdi:calculator",  //50
  "mdi:fridge-industrial-alert",
  "mdi:home-alert",
  "mdi:home-alert",
  "mdi:delta",
  "mdi:export",  //55
  "mdi:export",
  "mdi:radiator",
  "mdi:snowflake",
  "mdi:radiator-off",
  "mdi:snowflake-off",  //60
  "mdi:radiator-off",
  "mdi:snowflake-off",
  "mdi:waves-arrow-right",
  "mdi:waves-arrow-right",
  "mdi:thermostat",  //65
  "mdi:thermostat",
  "mdi:thermostat",
  "mdi:heat-pump-outline",
  "mdi:heat-pump-outline",
  "mdi:heat-pump-outline",  //70
  "mdi:transmission-tower-import",
  "mdi:transmission-tower-import",
  "mdi:alpha-v-circle-outline",
  "mdi:hand-water",
  "mdi:valve",  //75
  "mdi:chip",
  "mdi:thermometer-plus",
  "mdi:calculator",
  "mdi:water-thermometer",
  "mdi:heat-pump-outline",
  "mdi:water-thermometer",
  "mdi:water-thermometer",
  "mdi:water-thermometer",
  "mdi:water-thermometer",
  "mdi:fan",
  "mdi:fan",
  "mdi:valve",

  "mdi:thermostat",
  "mdi:thermostat",  //80
  "mdi:thermostat",
  "mdi:thermostat",
  "mdi:thermostat",

  "mdi:thermometer-plus",
  "mdi:power",  //85
  "mdi:bag-suitcase",
  "mdi:server-network-outline",
  "mdi:cancel",
  "mdi:cancel",
  "mdi:cancel",  //90
  "mdi:cancel",
  "mdi:cancel",
  "mdi:thermometer-plus",

  "mdi:auto-mode",
  "mdi:sun-snowflake-variant",
  "mdi:sun-snowflake-variant",
  "mdi:resize",
  "mdi:water-opacity"  //100
};


const char MQTT_SENSOR_NAME[][40] PROGMEM = {
  "Bridge Status",
  "Firmware Version",
  "Bridge WiFi Signal",
  "FTC Version",
  "System Operation Mode",
  "Heater Return Temperature",  //5
  "Heater Flow Temperature",
  "Outside Temperature",
  "Defrost",
  "Heat Pump Output Power",
  "Compressor Frequency",  // 10
  "Flow Rate",
  "Run Hours",
  "Max Flow Temperature",
  "Min Flow Temperature",
  "Boiler Flow Temperature",  //15
  "Boiler Return Temperature",
  "Mixing Tank Temperature",
  "Refrigerant Temperature",
  "Immersion Heater",
  "Booster Heater 1",  //20
  "Primary Water Pump",
  "Water Pump 2",
  "Three Way Valve 1",
  "Three Way Valve 2",
  "DHW Temperature",  //25
  "DHW Temperature Upper",
  "Prohibit DHW",
  "DHW Running",
  "DHW Control Mode",
  "Legionella Setpoint",  //30
  "DHW Max Temperature Drop",
  "DHW Heating Phase",
  "Zone 1 Flow Temperature",
  "Zone 1 Return Temperature",
  "Zone 2 Flow Temperature",  //35
  "Zone 2 Return Temperature",
  "Zone 1 Working",
  "Zone 2 Working",
  "Consumed Heating Yesterday",
  "Delivered Heating Yesterday",  //40
  "Consumed Cooling Yesterday",
  "Delivered Cooling Yesterday",
  "Consumed DHW Yesterday",
  "Delivered DHW Yesterday",
  "Total Consumed Energy Yesterday",  //45
  "Total Delivered Energy Yesterday",
  "Heating CoP Yesterday",
  "Cooling CoP Yesterday",
  "DHW CoP Yesterday",
  "Total CoP Yesterday",  //50
  "Refrigerant Fault Code",
  "Error Code",
  "Fault Code",
  "Flow Return DeltaT",
  "Computed Heat Output Power",  //55
  "Computed Cool Output Power",
  "Heating Running",
  "Cooling Running",
  "Zone 1 Heating Prohibit",
  "Zone 1 Cooling Prohibit",  //60
  "Zone 2 Heating Prohibit",
  "Zone 2 Cooling Prohibit",
  "Zone 1 Flow Setpoint",
  "Zone 2 Flow Setpoint",
  "Zone 1 Ext Thermostat Demand",  //65
  "Zone 2 Ext Thermostat Demand",
  "Outside Ext Thermostat Demand",
  "Water Pump 4",
  "Water Pump 3",
  "Water Pump 13",  // 70
  "Heat Pump Input Power",
  "Computed Input Power",
  "FTC Software Version",
  "DHW Boost",
  "Mixing Valve Step",  //75
  "Bridge CPU Temperature",
  "Booster Heater 2",
  "Instant CoP",
  "Refrigerant Liquid Temperature TH3",
  "Compressor Start Quantity",
  "Discharge Temperature TH4",
  "Subcool Temperature",
  "Heatsink Temperature TH8",
  "2 Phase Temperature TH6",
  "Fan 1 Speed",
  "Fan 2 Speed",
  "Valve LEV A",

  "DHW Thermostat",
  "Zone 1 Thermostat",  //80
  "Zone 2 Thermostat",
  "Zone 1 Flow Thermostat",
  "Zone 2 Flow Thermostat",

  "Fast DHW Boost",
  "System Power",  //85
  "Holiday Mode",
  "Server Control Mode",
  "Prohibit DHW",
  "Prohibit Zone 1 Heating",
  "Prohibit Zone 1 Cooling",  //90
  "Prohibit Zone 2 Heating",
  "Prohibit Zone 2 Cooling",
  "DHW Boost",

  "DHW Mode",
  "Heating/Cooling Operation Mode Zone 1",  //95
  "Heating/Cooling Operation Mode Zone 2",
  "Outdoor Unit Size (kW)",
  "Glycol Strength"
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
  "/Status/Configuration",              //9
  "/Command/System/HeatingMode",        //10
  "/Command/HotWater/Setpoint",         //11
  "/Command/Zone1/ThermostatSetpoint",  //12
  "/Command/Zone2/ThermostatSetpoint",  //13
  "/Command/HotWater/Boost",            //14
  "/Command/System/Power",              //15
  "/Command/System/HolidayMode",        //16
  "/Command/System/SvrControlMode",     //17
  "/Command/HotWater/Prohibit",         //18
  "/Command/Zone1/ProhibitHeating",     //19
  "/Command/Zone1/ProhibitCooling",     //20
  "/Command/Zone2/ProhibitHeating",     //21
  "/Command/Zone2/ProhibitCooling",     //22
  "/Command/HotWater/NormalBoost",      //23
  "/Command/Zone1/FlowSetpoint",        //24
  "/Command/Zone2/FlowSetpoint",        //25
  "/Command/HotWater/Mode",             //26
  "/Command/Zone1/HeatingMode",         //27
  "/Command/Zone2/HeatingMode",         //28
  "/Command/System/UnitSize",           //29
  "/Command/System/Glycol"              //30
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
  6,
  4
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
  2,  //10
  2,
  2,
  2,
  3,
  3,  //15
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
  2,
  2,
  3,
  3,  //60
  5,
  5,
  6,
  6,
  5,  //65
  6,
  8,
  8,
  8,
  8,
  8,
  8,
  2,
  2,
  1,
  4,
  3,
  1,
  3,
  2,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9
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
  3,
  3,
  0,
  0,  //60
  0,
  0,
  0,
  0,
  2,  //65
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  3,
  3,
  0,
  0,
  0,
  2,
  0,
  8,
  2,
  12,
  2,
  2,
  2,
  2,
  10,
  10,
  11
};

const char MQTT_OBJECT_ID[][19] PROGMEM = {
  "dhw_climate",
  "zone1_climate",
  "zone2_climate",
  "zone1_flow_climate",
  "zone2_flow_climate"
};


int MQTT_CLIMATE_MAX[] PROGMEM = {
  60,
  30,
  30,
  60,
  60
};

int MQTT_CLIMATE_MIN[] PROGMEM = {
  0,
  10,
  10,
  10,
  10
};

float MQTT_CLIMATE_TEMP_STEP[] PROGMEM = {
  0.5,
  0.5,
  0.5,
  0.5,
  0.5
};

float MQTT_CLIMATE_PRECISION[] PROGMEM = {
  0.5,
  0.5,
  0.5,
  0.5,
  0.5
};

int MQTT_CLIMATE_INITAL[] PROGMEM = {
  50,
  10,
  10,
  30,
  30
};

const char MQTT_CLIMATE_MODE[][120] PROGMEM = {
  "{%set values = {'heat':'Heating Temperature','cool':'Cooling Temperature'}%}{{values[value] if value in values.keys()}}"
};

const char MQTT_CLIMATE_STATE_TOPIC[][360] PROGMEM = {
  "{{'heat' if states('sensor.ecodan_ashp_prohibit_dhw')=='0' or states('sensor.ecodan_ashp_dhw_heating_phase')!='Off' else 'off'}}",
  "{{'heat' if (value_json.OpMode=='Heat' and states('sensor.ecodan_ashp_zone_1_heating_prohibit')=='0') else 'cool' if (value_json.OpMode=='Cool' and states('sensor.ecodan_ashp_zone_1_heating_prohibit')=='0') else 'off'}}",
  "{{'heat' if (value_json.OpMode=='Heat' and state_attr('climate.zone2_climate','current_temperature')!=0 and states('sensor.ecodan_ashp_zone_2_heating_prohibit')=='0') else 'cool' if (value_json.OpMode=='Cool' and state_attr('climate.zone2_climate','current_temperature')!=0 and states('sensor.ecodan_ashp_zone_2_cooling_prohibit')=='0') else 'off'}}",
  "{{'heat' if (value_json.OpMode=='Heat' and states('sensor.ecodan_ashp_zone_2_heating_prohibit')=='0') else 'cool' if (value_json.OpMode=='Cool' and states('sensor.ecodan_ashp_zone_1_cooling_prohibit')=='0') else 'off'}}",
  "{{'heat' if (value_json.OpMode=='Heat' and state_attr('climate.zone2_climate','current_temperature')!=0 and states('sensor.ecodan_ashp_zone_2_heating_prohibit')=='0') else 'cool' if (value_json.OpMode=='Cool' and state_attr('climate.zone2_climate','current_temperature')!=0 and states('sensor.ecodan_ashp_zone_2_cooling_prohibit')=='0') else 'off'}}"
};


const char MQTT_CLIMATE_MODE_STATE_TEMPLATE[][417] PROGMEM = {
  "{{'heating' if value_json.SystemOperationMode in ['Hot Water','Legionella'] else 'defrosting' if value_json.SystemOperationMode in ['Defrosting','Frost Protect'] else 'idle' if states('sensor.ecodan_ashp_prohibit_dhw')!='1' else 'off'}}",
  "{%set mode=value_json.SystemOperationMode|lower%}{%set h_prhbt=not(states('sensor.ecodan_ashp_zone_2_heating_prohibit')|bool)%}{%set c_prhbt=not(states('sensor.ecodan_ashp_zone_1_cooling_prohibit')|bool)%}{%set wrkg=states('sensor.ecodan_ashp_zone_1_working')|bool%}{%if mode in ['defrosting','frost protect']%}defrosting{%elif ((h_prhbt or c_prhbt) and not(wrkg)) or mode=='hot water'%}idle{%else%}{{mode}}{%endif%}",
  "{%set mode=value_json.SystemOperationMode|lower%}{%set h_prhbt=not(states('sensor.ecodan_ashp_zone_2_heating_prohibit')|bool)%}{%set c_prhbt=not(states('sensor.ecodan_ashp_zone_2_cooling_prohibit')|bool)%}{%set wrkg=states('sensor.ecodan_ashp_zone_2_working')|bool%}{%if mode in ['defrosting','frost protect']%}defrosting{%elif ((h_prhbt or c_prhbt) and not(wrkg)) or mode=='hot water'%}idle{%else%}{{mode}}{%endif%}",
  "{%set mode=value_json.SystemOperationMode|lower%}{%set h_prhbt=not(states('sensor.ecodan_ashp_zone_2_heating_prohibit')|bool)%}{%set c_prhbt=not(states('sensor.ecodan_ashp_zone_1_cooling_prohibit')|bool)%}{%set wrkg=states('sensor.ecodan_ashp_zone_1_working')|bool%}{%if mode in ['defrosting','frost protect']%}defrosting{%elif ((h_prhbt or c_prhbt) and not(wrkg)) or mode=='hot water'%}idle{%else%}{{mode}}{%endif%}",
  "{%set mode=value_json.SystemOperationMode|lower%}{%set h_prhbt=not(states('sensor.ecodan_ashp_zone_2_heating_prohibit')|bool)%}{%set c_prhbt=not(states('sensor.ecodan_ashp_zone_2_cooling_prohibit')|bool)%}{%set wrkg=states('sensor.ecodan_ashp_zone_2_working')|bool%}{%if mode in ['defrosting','frost protect']%}defrosting{%elif ((h_prhbt or c_prhbt) and not(wrkg)) or mode=='hot water'%}idle{%else%}{{mode}}{%endif%}",
};

const char MQTT_SELECT_VALUE_TEMPLATE[][405] PROGMEM = {
  "{{'Normal' if value_json.HotWaterControlMode=='Normal' else 'Eco' if value_json.HotWaterControlMode=='Eco'}}",
  "{{'Heating Temperature' if value_json.HeatingControlMode=='Temp' else 'Heating Flow' if value_json.HeatingControlMode=='Flow' else 'Heating Compensation' if value_json.HeatingControlMode=='Compensation' else 'Cooling Temperature' if value_json.HeatingControlMode=='Cool' else 'Cooling Flow' if value_json.HeatingControlMode=='Cool Flow' else 'Dry Up' if value_json.HeatingControlMode=='Dry Up'}}",
  "{{'Heating Temperature' if value_json.HeatingControlMode=='Temp' else 'Heating Flow' if value_json.HeatingControlMode=='Flow' else 'Heating Compensation' if value_json.HeatingControlMode=='Compensation' else 'Cooling Temperature' if value_json.HeatingControlMode=='Cool' else 'Cooling Flow' if value_json.HeatingControlMode=='Cool Flow' else 'Dry Up' if value_json.HeatingControlMode=='Dry Up'}}",
  "{{value_json.UnitSize}}",
  "{{value_json.Glycol}}"
};

const char MQTT_SENSOR_UNITS[][7] PROGMEM = {
  "",
  "dBm",
  "°C",
  "kW",
  "Hz",
  "l/min",
  "hrs",
  "kWh",
  "CoP",
  "C",
  "rpm",
  "Pulses",
  "Times"
};

const char MQTT_NUMBER_AVAIL_TEMPLATE[][90] PROGMEM = {
  "{{'online' if value_json.HeatingControlMode in ['Flow','Cooling Flow'] else 'offline'}}"
};

const char MQTT_SENSOR_VALUE_TEMPLATE[][50] PROGMEM = {
  "{{ value if value is defined else 'Unknown' }}",
  "{{ value_json.Firmware }}",
  "{{ value_json.RSSI }}",
  "{{ value_json.FTCVersion }}",
  "{{ value_json.SystemOperationMode }}",
  "{{ value_json.HeaterReturn|round(1) }}",  //5
  "{{ value_json.HeaterFlow|round(1) }}",
  "{{ value_json.OutsideTemp }}",
  "{{ value_json.Defrost }}",
  "{{ value_json.HeaterPower }}",
  "{{ value_json.Compressor }}",  //10
  "{{ value_json.FlowRate }}",
  "{{ value_json.RunHours }}",
  "{{ value_json.FlowTMax }}",
  "{{ value_json.FlowTMin }}",
  "{{ value_json.BoilerFlow }}",  //15
  "{{ value_json.BoilerReturn }}",
  "{{ value_json.MixingTemp }}",
  "{{ value_json.RefrigeTemp }}",
  "{{ value_json.Immersion }}",
  "{{ value_json.Booster }}",  //20
  "{{ value_json.PrimaryWaterPump }}",
  "{{ value_json.WaterPump2 }}",
  "{{ value_json.ThreeWayValve }}",
  "{{ value_json.ThreeWayValve2 }}",
  "{{ value_json.Temperature|round(1) }}",  //25
  "{{ value_json.TempTHW5A|round(1) }}",
  "{{ value_json.ProhibitDHW }}",
  "{{ value_json.DHWActive }}",
  "{{ value_json.HotWaterControlMode }}",
  "{{ value_json.LegionellaSetpoint }}",  //30
  "{{ value_json.HotWaterMaxTDrop }}",
  "{{ value_json.HotWaterPhase }}",
  "{{ value_json.FlowTemp }}",
  "{{ value_json.ReturnTemp }}",
  "{{ value_json.FlowTemp }}",  //35
  "{{ value_json.ReturnTemp }}",
  "{{ value_json.TwoZone_Z1Working }}",
  "{{ value_json.TwoZone_Z2Working }}",
  "{{ value_json.CHEAT|round(2) }}",
  "{{ value_json.DHEAT|round(2) }}",  //40
  "{{ value_json.CCOOL|round(2) }}",
  "{{ value_json.DCOOL|round(2) }}",
  "{{ value_json.CDHW|round(2) }}",
  "{{ value_json.DDHW|round(2) }}",
  "{{ value_json.CTOTAL|round(2) }}",  //45
  "{{ value_json.DTOTAL|round(2) }}",
  "{{ value_json.HEAT_CoP|round(2) }}",
  "{{ value_json.COOL_CoP|round(2) }}",
  "{{ value_json.DHW_CoP|round(2) }}",
  "{{ value_json.TOTAL_CoP|round(2) }}",  //50
  "{{ value_json.RefrigeFltCode }}",
  "{{ value_json.ErrCode }}",
  "{{ value_json.FltCode }}",
  "{{ value_json.FlowReturnDeltaT }}",
  "{{ value_json.EstHeatOutputPower }}",  //55
  "{{ value_json.EstCoolOutputPower }}",
  "{{ value_json.HeatingActive }}",
  "{{ value_json.CoolingActive }}",
  "{{ value_json.ProhibitHeating }}",
  "{{ value_json.ProhibitCooling }}",  //60
  "{{ value_json.ProhibitHeating }}",
  "{{ value_json.ProhibitCooling }}",
  "{{ value_json.FSP }}",
  "{{ value_json.FSP }}",
  "{{ value_json.Z1TstatDemand }}",  //65
  "{{ value_json.Z2TstatDemand }}",
  "{{ value_json.OTstatDemand }}",
  "{{ value_json.WaterPump4 }}",
  "{{ value_json.WaterPump3 }}",
  "{{ value_json.WaterPump13 }}",  //70
  "{{ value_json.InputPower }}",
  "{{ value_json.EstInputPower }}",
  "{{ value_json.FTCSoftwareVersion }}",
  "{{ value_json.HotWaterEcoBoostActive }}",
  "{{ value_json.MixingStep }}",  //75
  "{{ value_json.CPUTemp }}",
  "{{ value_json.Booster2 }}",
  "{{ value_json.Instant_CoP }}",
  "{{ value_json.LiquidTemp }}",
  "{{ value_json.CompOpTimes }}",
  "{{ value_json.TH4Discharge }}",
  "{{ value_json.Subcool }}",
  "{{ value_json.TH8HeatSink }}",
  "{{ value_json.TH6Pipe }}",
  "{{ value_json.Fan1RPM }}",
  "{{ value_json.Fan2RPM }}",
  "{{ value_json.LEVA }}",

  "{{ value_json }}",
  "{{ value_json.Setpoint }}",  //80
  "{{ value_json.FSP }}",
  "{{ value_json.HotWaterBoostActive }}",
  "{{ value_json.SystemPower }}",
  "{{ value_json.HolidayMode }}",
  "{{ value_json.SvrControlMode }}",  //85
  "{{ value_json.ProhibitDHW }}",
  "{{ value_json.ProhibitHeating }}",
  "{{ value_json.ProhibitCooling }}",
  "{{ value_json.ProhibitHeating }}",
  "{{ value_json.ProhibitCooling }}",  //90
  "{{ value_json.HotWaterEcoBoostActive }}",
  "{{ value_json.HeatingControlMode }}"
};

const char MQTT_DISCOVERY_TOPICS[][23] PROGMEM = {
  "homeassistant/sensor/",
  "homeassistant/climate/",
  "homeassistant/switch/",
  "homeassistant/climate/",
  "homeassistant/select/",
  "/config"
};

const char MQTT_DEVICE_CLASS[][7] PROGMEM = {
  "energy"
};