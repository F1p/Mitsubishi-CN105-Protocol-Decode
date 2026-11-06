# Home Assistant Parameters Detailed Explanations

# Control
| Name | Options      | Description of Function      |
|-------|------------------|----------------|
|  DHW Boost | On/Off      |  The Ecodan will perform a Normal or Eco Hot Water run at the transition of any prohibited to not prohibited, regardless of the temperature drop, this switch utilises this function to run. When the hot water run finishes the bridge puts the previous Server Control Mode and prohibit settings back, and firmware updates are held off while it is running |
|  Fast DHW Boost | On/Off      | This is the Ecodan's Forced Hot Water mode, compressor frequency and output power are maximum - compromising efficiency for speed to heat hot water at maximum speed |
|  Short Cycle Protection | On/Off | Turns on the short cycle lockout, anti-stop flow temperature following and the hot water flow following described in [Short Cycle Protection (Active Control)](#short-cycle-protection-active-control). Off by default, the setting is saved and survives a restart. Air to Water only |
|  Glycol Strength | 0%/10%/15%/20%/25%/30%  | The heat coefficient of water varies with the strength of glycol used, the sensor "Computed Heat Output" will use this variable to determine the coefficient of water to be used (specific heat of 4.18, 4.12, 4.10, 4.07, 3.98 and 3.90 kJ/kg·K respectively)  |
|  Outdoor Unit Size (kW) | 4.0, 5.0, 6.0, 7.5, 8.0, 8.5, 10.0, 11.2, 12.0, 14.0, 23.0  | The Computed Input Power varies depending on the outdoor unit size, to determine a suitable multiplier this value is used. The bridge also reads the outdoor unit's capacity code (service code 70) and corrects this value automatically if it does not fit the detected unit |
|  Zone Operation Mode | Heating Temperature (Heating Auto Adapt), Heating Flow (Heating Fixed Flow), Heating Compensation (Heating Weather Compensation), Cooling Temperature (Cooling Auto Adapt), Cooling Flow (Cooling Fixed Flow), Cooling Compensation (Cooling Weather Compensation), Dry Up  | Some configurations require Zone1 & Zone2 to be fixed to the same control mode, Zone2 may not be possible to modify if this is your case |
|  Server Control Mode | On/Off  | Server Control Mode is required to control the Prohibits over this protocol, entering Server Control Mode will ignore any schedules for prohibits in the FTC. A globe symbol appears on your Main controller |
|  Prohibit DHW, Prohibit Zone 1 Heating, Prohibit Zone 1 Cooling, Prohibit Zone 2 Heating, Prohibit Zone 2 Cooling | On/Off | Stops the FTC running that function while on. These can only be changed while Server Control Mode is on. The Short Cycle Protection lockout uses the zone prohibits |
|  System Power | On/Standby | Puts the FTC into or out of standby |
|  Holiday Mode | On/Off | Switches the FTC's Holiday Mode on or off |
|  DHW Mode | Normal/Eco | The FTC's hot water operating mode |
|  Thermostats (DHW, Zone 1, Zone 2) | Temperature | The target temperature for hot water (0-60C, 70C on R290 units, 0.5C steps) and for each zone's room temperature (10-30C, 0.5C steps) |
|  Flow Thermostats | On/Off  | You must be in a Flow control Mode to change flow thermostats, current temperature can be obtained from the sensors. The flow setpoint range is 5-60C in 0.1C steps |
|  MELCloud Read Only | On/Off | While on, the bridge does not pass any write requests from MELCloud to the FTC (see service codes 997 and 996). Only available while a MELCloud adapter is connected |
|  Firmware Update | Install | Installs the latest firmware over the air (ESP32 hardware only), the same as service code 995. Held off while a DHW Boost is running |
|  Web Interface Password | Text (4-32 characters) | Sets the password for the bridge's onboard web page (user name `admin`). Printable ASCII characters only. Entering nothing turns the login off. The stored password is never published, the entity only shows `********` or `disabled`. Until a password is set the login is the Device ID (shown as the Serial Number in Home Assistant). Ethernet (WT32-ETH01) builds have no login until a password is set |

# Sensors
| Name | Unit      | Description of Function      |
|-------|------------------|----------------|
|   Heat Pump Input Power | kW (floor/ceiling)      | The value provided by the FTC for input power (to 0 d.p)    |
|   Heat Pump Computed Input Power | kW      | The estimated power input calculated by the bridge controller, based on the compressor frequency, the flow temperature and the unit size set in the control section. With the compressor off it is a small standby figure. While an immersion or booster heater is running on its own, the power reported by the FTC is used instead   |
|   Heat Pump Output Power | kW (floor/ceiling)      | The value provided by the FTC for output power (to 0 d.p)    |
|   Heat Pump Computed Heat Output Power | kW       | The estimated Heating power output calculated by the bridge controller (flow rate x flow/return temperature difference x the specific heat of the fluid), based on the glycol strength set in the control section   |
|   Heat Pump Computed Cool Output Power | kW       | The estimated Cooling power output calculated by the bridge controller, based on the glycol strength set in the control section  |
|   Computed Heating / DHW / Cooling Input & Output Power | kW | The computed input and output power attributed to whichever mode is running: space heating, hot water or cooling. During a defrost the input counts as heating and the heat taken out of the water counts as cooling output. These are what the "Total CoP Today" figure is built from |
|   Instant CoP | n/a       | The estimated Instant CoP computed by the bridge, based on Computed Input & Output Power. It is only calculated while the compressor is running (or an immersion/booster is running on its own) and the computed input is at least 0.05 kW, otherwise it shows 0. It is limited to +/-15 to hide short spikes during compressor ramp-up or valve changeover. It reads negative while heat is being taken out of the water (for example a defrost), and during space cooling the heat removed counts as the useful output so the figure reads as a positive EER |
|   Total CoP Today | n/a | The CoP for the day so far, calculated by the bridge from the computed input and output power (delivered energy divided by consumed energy). It resets at midnight on the FTC clock and the finished day then becomes the bridge's own "yesterday" figures |
|   "Yesterday" figures | n/a       | The energy consumed/delivered & CoP, estimated by the FTC and matching the stats on the Main Controller. For a few minutes around midnight they are not published. If the FTC has not yet reported yesterday's figures, the bridge's own totals are used in their place |
|   Zone 1 / Zone 2 Flow Setpoint | degC | The flow temperature setpoint the FTC is working to (FSP). With Short Cycle Protection or the Onboard Compensation Curve active, this is the value the bridge last wrote, so it can differ from the setpoint you entered while flow following is active |
|   Short Cycle Protection State | n/a | What Short Cycle Protection is doing right now, see [Reading the status](#reading-the-status) |

# Short Cycle Protection (Active Control)
A heat pump short cycles when the compressor stops and restarts again within a few minutes, which is hard on the compressor and wastes energy. It is most common in mild weather or when cooling with a low demand. Short Cycle Protection (called *Active Control* in the MQTT topic names) has four parts:

1. **Short cycle lockout** - detects the heat pump short cycling and temporarily prohibits space heating and cooling
2. **Anti-stop flow temperature following** - nudges the flow setpoint up (or down when cooling) to stop the FTC cutting the outdoor unit
3. **DHW flow following** - keeps the flow setpoint matched to the flow temperature while hot water is being heated
4. **Post DHW letdown** - brings the flow temperature smoothly back down to the compensation curve after hot water

None of these take any action unless the **Short Cycle Protection** switch is on. It only applies to Air to Water heat pumps (FTC), not Air to Air units. Parts 3 and 4 also need the Onboard Compensation Curve to be active, see [What each part needs](#what-each-part-needs).

In the MQTT examples below, `<Base Topic>` is the Primary MQTT Base Topic from the bridge's setup page (the same prefix used in the Service Codes examples further down, for example `Ecodan/ASHP`).

## Settings
| Setting | How to change it | Default | Description |
|-------|------------------|----------------|----------------|
| Short Cycle Protection | The Home Assistant switch, MQTT `<Base Topic>/Command/System/ActiveControl` with `1` or `0`, or service code 992 (toggles) | Off | Turns all four parts on or off. Turning it off while a lockout is running ends the lockout straight away |
| Max Flow Overshoot | Service codes 880 to 900 (880 = 0C, 900 = 20C), or `max_flow_overshoot` in the CompCurve JSON | 3C | The most the flow setpoint may be moved from its original value by anti-stop flow following, in 0.5C steps (3C allows 6 steps) |
| Fixed Lockout Duration | `fixedlockoutduration` in the CompCurve JSON, in **minutes** | 0 (Automatic) | Use a fixed lockout time instead of the automatic one. 0 returns to Automatic. Available from firmware v7.0.24 |

All three settings are saved on the bridge and survive a restart.

The fixed lockout duration has no Home Assistant entity, it is sent as JSON to `<Base Topic>/Command/System/CompCurve`:

```
{"fixedlockoutduration": 30}
```

Sets a fixed lockout of 30 minutes. Send `{"fixedlockoutduration": 0}` to go back to Automatic. In the same way, `{"max_flow_overshoot": 2}` sets the Max Flow Overshoot to 2C (this is the same as sending service code 882).

From a Home Assistant automation or script:

```yaml
action: mqtt.publish
data:
  topic: "Ecodan/ASHP/Command/System/CompCurve"   # use your own Base Topic
  payload: '{"fixedlockoutduration": 30}'
```

## 1. Short cycle lockout

**Detection.** The bridge times the gap between each compressor stop and the next (stops that happen during a defrost are ignored). When the last two of those gaps add up to **less than 20 minutes** (an average of under 10 minutes between stops, so more than 6 stops an hour) a short cycle is declared. The check is made once each time the compressor stops (and once when you switch Short Cycle Protection on, using the last two periods already recorded). The *Short Cycle Reason* is recorded and the status published even while the switch is off, but the lockout itself only happens when the switch is on.

**Lockout.** The bridge puts the FTC into Server Control Mode and prohibits **Zone 1 and Zone 2 heating and cooling**. The hot water prohibit is left as it was, so hot water can still run. Without space heating or cooling the compressor stops and gets a rest. The lockout is not started if hot water and all four zone prohibits are already on. Because it uses Server Control Mode, the globe symbol appears on your Main controller while it is active.

**Lockout duration.** By default the duration is Automatic, worked out from how fast the heat pump was cycling:

`lockout (minutes) = 10 + (combined length of the last two periods in seconds / 120)`

| Last two compressor periods added together | Automatic lockout |
|-------|------------------|
| Close to 0 minutes | 10 minutes |
| 5 minutes | 12.5 minutes |
| 10 minutes | 15 minutes |
| 15 minutes | 17.5 minutes |
| Just under 20 minutes | Just under 20 minutes |

If you set a Fixed Lockout Duration it replaces the automatic value, whatever the cycling speed. A fixed duration is useful when the automatic 10-20 minutes is not long enough, which can happen when cooling with a low demand. Bear in mind that no space heating or cooling happens for the whole lockout.

**Release.** When the lockout time is up, or as soon as you turn Short Cycle Protection off, the bridge puts back the Server Control Mode and prohibit settings that were in place before the lockout. The compressor history is cleared at the same time, so detection starts again from scratch.

**Short Cycle Reason.** Records what the heat pump was doing when the short cycle was detected:

| Reason | When it is set |
|-------|------------------|
| Flow Temp Exceeded Flow Setpoint | The flow temperature was more than 4C away from the Zone 1 flow setpoint |
| Thermostat Demand | Otherwise, if the FTC's system operation mode was Off (idle) at that moment |
| (blank) | Neither applies, or no lockout has happened |

## 2. Anti-stop flow temperature following

The FTC stops the outdoor unit if the flow temperature goes more than 2.0C above the flow setpoint, and the flow and return sensors only read in 0.5C steps. If the flow overshoots the setpoint the compressor is stopped, then starts again soon after, which is a short cycle. This part moves the setpoint just far enough to keep the flow within the FTC's limit.

It applies to **Zone 1 only**, while the compressor is running in heating or cooling operation and the Zone 1 flow setpoint is above 0. For heating, Zone 1 must be in Heating Flow mode. For cooling, Zone 1 must be in Cooling Flow mode. It is paused during the post DHW letdown. It works like this:

| Step | Heating | Cooling |
|-------|------------------|----------------|
| Trigger | Flow is more than 1.0C **above** the setpoint and still rising | Flow is more than 1.0C **below** the setpoint and still falling |
| Action | Setpoint raised by 0.5C | Setpoint lowered by 0.5C |
| Limit | Up to Max Flow Overshoot in total (3C by default, so 6 steps) | Same |
| Step back | Once the flow is less than 0.5C above the raised setpoint, the setpoint is lowered by 0.5C | Once the flow is less than 0.5C below the lowered setpoint, the setpoint is raised by 0.5C |

The setpoint from before the first step is remembered (*FlowTemp_Target*). When the compressor stops, the bridge hands the setpoint back, to the compensation curve value if the Onboard Compensation Curve is driving Zone 1, otherwise to the remembered setpoint, and resets the step counter. The compensation curve does not write a new Zone 1 flow setpoint while following is in progress.

## 3. DHW flow following

While hot water is being heated the flow temperature rises far above the heating setpoint. If the setpoint was left alone, the FTC would stop the outdoor unit when it switched back to space heating, because the flow would be more than 2.0C above it.

Every 30 seconds while hot water is heating, the bridge sets the Zone 1 flow setpoint to the actual flow temperature. This only happens when Zone 1 is in Heating Flow mode and the Onboard Compensation Curve is active for Zone 1 or Zone 2.

## 4. Post DHW letdown

After hot water finishes the flow temperature falls slowly, at a rate that depends on your installation. Holding the setpoint **above** the flow only keeps the compressor working against a target it cannot reach, so instead the bridge holds the setpoint **0.5C below** the actual flow temperature. The FTC then reduces the compressor frequency and the flow comes down towards the curve, while staying inside the FTC's 2.0C stop limit.

The bridge hands control back to the Onboard Compensation Curve as soon as the flow is within 0.5C of the curve's flow setpoint (straight away if it already is when hot water ends). If that has not happened after 15 minutes it is handed back anyway. If hot water starts again, DHW flow following takes over again.

The letdown only starts if DHW flow following was running during that hot water run, so a very short run (under 30 seconds) does not start it. It needs Zone 1 in Heating Flow mode and the Onboard Compensation Curve active for Zone 1. If only Zone 2's curve is active, control is handed straight back to the compensation curve when hot water ends.

## What each part needs
| Part | Short Cycle Protection switch | Onboard Compensation Curve | Other conditions |
|-------|------------------|----------------|----------------|
| Short cycle lockout | On to take action (detection runs regardless) | Not needed | Compressor stops detected |
| Anti-stop flow following | On | Not needed | Compressor running in heating (Zone 1 in Heating Flow mode) or cooling (Zone 1 in Cooling Flow mode) operation |
| DHW flow following | On | Zone 1 or Zone 2 active | Zone 1 in Heating Flow mode, hot water heating |
| Post DHW letdown | On | Zone 1 active | Zone 1 in Heating Flow mode, DHW flow following ran during the hot water run |

## Reading the status
The **Short Cycle Protection State** sensor shows one of the following. If more than one applies, the one nearest the top of the list is shown:

| Value | Meaning |
|-------|------------------|
| Short Cycle Lockout Active | Space heating and cooling are prohibited while the lockout runs |
| Post DHW Flow Temperature Letdown Active | The flow setpoint is being held 0.5C below the flow after hot water |
| DHW Flow Temperature Following Active | The flow setpoint is following the flow temperature during hot water |
| Anti-Stop Flow Temperature Following Active (1.50C) | The setpoint is currently moved by this much (0.5C per step) from where it started |
| Inactive | Nothing is running |

The full set of values is published on the topic `<Base Topic>/Status/ActiveControl`, which you can read in MQTT Explorer:

| Field | Description |
|-------|------------------|
| ShortCycleProtectionEnabled | 1 when the switch is on, otherwise 0 |
| ShortCycleProtectionActive | The state text from the table above |
| ShortCycleReason | The reason recorded when the short cycle was detected |
| FixedShortCycleLockoutDuration | "Disabled (Automatic)", or the fixed duration in minutes |
| ShortCycleLockoutDuration | The length of the current or last lockout in **milliseconds**, back to 0 once it has been released |
| LastCompressorPeriods | The last two compressor periods in seconds, newest first (stop to stop) |
| FlowTemp_Target | The setpoint remembered when anti-stop flow following started |
| FlowIncrementCounter | How many 0.5C steps anti-stop flow following has applied now |

The compressor history and the lockout timer are only held in memory, so they start again after the bridge restarts.

# Service Codes
These depend on your outdoor unit and can be located within the [Service Manuals specific to your unit](https://www.manualslib.com/manual/795612/Mitsubishi-Electric-Par-21maa.html?page=51)

*Using MQTT Explorer or Home Assistant Automations, you can publish Service Codes to: Ecodan/ASHP/Command/System/Svc*

*Results will be allocated to their standard topic location, or for ones not published the topic: Ecodan/ASHP/Status/AdvancedTwo under "LastSvc": 995 and "LastSvcReply": 0*

This can be used to poll certain outdoor sensors at an increased rate, warning, this may reduce the speed of other operations



| Request Code | Name | Unit      | Description of Function      |
|-------|------------------|----------------|----|
| 3 |   Compressor-Number of operation times      | 100 Times  | Home Assistant: Compressor Start Quantity |
| 4 | Discharge temperature (TH4) | degC      | Home Assistant: Discharge Temperature TH4    |
| 5 | Outdoor unit -Liquid pipe 1 temperature (TH3) | degC      | Home Assistant: Refrigerant Liquid Temperature TH3    |
| 6 | Water inlet temperature (TH32) | degC      | Home Assistant: Water Inlet Temperature TH32    |
| 7 | Outdoor unit-Plate HEX pipe temperature (TH6)  | degC      | Home Assistant: Plate Hex Liquid Temperature TH6    |
| 8 | Outdoor unit-Suction pipe temperature (TH33) | degC      | Published as TH33 on the Configuration topic, no Home Assistant entity    |
| 9 | Outdoor unit-Ambient temperature (TH7) | degC      | The reply is shown as LastSvcReply    |
| 10 | Outdoor unit-Heatsink temperature (TH8) | degC      | Home Assistant: Heatsink Temperature TH8    |
| 12 | Discharge superheat (SHd) | degC      | Home Assistant: Superheat Temperature    |
| 13 | Sub-cool (SC) | degC      | Home Assistant: Subcool Temperature    |
| 14 | Condensing temperature (T63HS) | degC      | Published as CondensingTemp on the Advanced topic, no Home Assistant entity    |
| 19 | Outdoor unit-Fan 1 speed | rpm      | Home Assistant: Fan 1 Speed    |
| 20 | Outdoor unit-Fan 2 speed | rpm      | Home Assistant: Fan 2 Speed    |
| 22 | LEV (A) opening | Pulses  | Home Assistant: Valve LEV A    |
| 23 | LEV (B) opening | Pulses      | Home Assistant: Valve LEV B    |
| 24 | LEV (C) opening | Pulses      | Only on multi-compressor R290 units, a reading other than 5 marks the unit as dual compressor    |
| 27 | Water inlet temperature (TH32) - Geodan | degC      | Used in place of code 6 on Geodan units    |
| 28 | Water outlet temperature (TH34) - Geodan | degC      | Used in place of code 23 on Geodan units, shown as the Brine Outlet Temperature    |
| 70 | Outdoor unit capacity code | n/a      | Used to check the Outdoor Unit Size setting (for example 9 = 3.5kW, 10 = 5kW, 11 = 6kW, 14 = 8.5kW, 20 = 10kW)    |
| 90 | Outdoor unit software version | n/a      | Home Assistant: Outdoor Unit Software Version    |

# Adapter Special Service Codes
In addition, there are special service codes
| Request Code | Name |  Description of Function      | Applicable Firmware | 
|-------|------------------|-----------|---------|
| 999 |   Restart Adapter      |  The Adapter will perform a restart  | All|
| 998 |   Disconnect from FTC      |  The Adapter will close connection gracefully with the FTC  |Home Assistant Only|
| 997 |   Block Write Requests from MELCloud      |  The Adapter will not pass on any write requests from MELCloud to the FTC  | All (In MELPump, Code 997 toggles this both on/off)|
| 996 |   Enable Write Requests from MELCloud      |  The Adapter will allow any write requests from MELCloud to the FTC (Default Action)  | Home Assistant|
| 995 |   Install Firmware Update      |  The Adapter will download and install the latest firmware, the same as the Firmware Update entity. A warning is shown if the WiFi signal is weaker than -75dBm. It will not start while a DHW Boost is running or when you are already on the latest version  | Home Assistant (ESP32) |
| 994 |   Check for OTA Updates      |  The Adapter will Check the Server for any Updates | Home Assistant (ESP32), MELPump |
| 993 |   Poll FTC For Version Information      |  Normally performed at the start, this ad-hoc method can be used to call the FTC for version information | All |
| 992 |   Short Cycle Protection      |  Toggles the Short Cycle Protection (Onboard Compensation Curve Mode) On/Off, the same as the Short Cycle Protection switch. LastSvcReply shows 1 for On and 0 for Off | Home Assistant, MELPump |
| 880 to 900 |   Short Cycle Protection Max. Flow. Overshoot      |  The Maximum Permitted Overshoot when running Short Cycle Protection where 880 = 0C and 900 = 20C. LastSvcReply shows the value in C | Home Assistant, MELPump |
