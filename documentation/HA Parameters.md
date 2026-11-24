# Home Assistant Parameters Detailed Explanations

# Control
| Name | Options      | Description of Function      |
|-------|------------------|----------------|
|  DHW Boost | On/Off      |  The Ecodan will perform a Normal or Eco Hot Water run at the transition of any prohibited to not prohibited, regardless of the temperature drop, this switch utilises this function to run. When the hot water run finishes the bridge puts the previous Server Control Mode and prohibit settings back, and firmware updates are held off while it is running |
|  Fast DHW Boost | On/Off      | This is the Ecodan's Forced Hot Water mode, compressor frequency and output power are maximum - compromising efficiency for speed to heat hot water at maximum speed |
|  Short Cycle Protection | On/Off | Turns on anti-stop flow temperature following (including floor-aware following), hot water flow following, the post hot water letdown and the short cycle lockout described in [Short Cycle Protection (Active Control)](#short-cycle-protection-active-control). Off by default, the setting is saved and survives a restart. Air to Water only |
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
The FTC stops the outdoor unit as soon as the flow temperature is more than **2.0C above the flow setpoint (FSP)**, and the flow and return sensors only read in 0.5C steps. The compressor also has a minimum (floor) frequency, so in mild weather it can produce more heat than the emitters can pass. The flow then rises, the FTC stops the outdoor unit, and it starts again a few minutes later: a short cycle. This is hard on the compressor and wastes energy.

Short Cycle Protection (called *Active Control* in the MQTT topic names) has five parts:

1. **Anti-stop flow following (heating)** - moves the Zone 1 flow setpoint up and down so it trails the actual flow, keeping it inside the FTC's 2.0C stop limit. It is aware of the compressor's floor frequency
2. **Anti-stop flow following (cooling)** - the same idea in 0.5C steps while cooling
3. **DHW flow following** - keeps the flow setpoint matched to the flow temperature while hot water is being heated
4. **Post DHW letdown** - brings the flow temperature smoothly back down to the compensation curve after hot water
5. **Short cycle lockout** - if short cycling still happens, temporarily prohibits space heating and cooling

None of these take any action unless the **Short Cycle Protection** switch is on. It only applies to Air to Water heat pumps (FTC), not Air to Air units, and all flow setpoint changes are made to **Zone 1**. Parts 3 and 4 also need the Onboard Compensation Curve to be active, see [What each part needs](#what-each-part-needs). The same information is in the [Onboard Compensation Curve](OnboardCompCurve.md) documentation.

In the MQTT examples below, `<base topic>` is the Primary MQTT Base Topic from the bridge's setup page, for example `Ecodan/ASHP` or `ASHP`. If you use a second MQTT server, use its Secondary MQTT Base Topic.

## Settings
| Setting | How to change it | Default | Description |
|-------|------------------|----------------|----------------|
| Short Cycle Protection | The Home Assistant switch, MQTT `<base topic>/Command/System/ActiveControl` with `1` or `0`, or service code 992 (toggles) | Off | Turns all five parts on or off. Turning it off while a lockout is running ends the lockout straight away |
| Max Flow Overshoot | `max_flow_overshoot` in the CompCurve JSON, or service codes 880 to 900 in whole degrees (880 = 0C, 900 = 20C) | 3C | The most the flow setpoint may be raised above its starting point (normally the curve) by anti-stop flow following |
| Floor Allowance | `floor_allowance` in the CompCurve JSON, 0 to 15C | 0 (Off) | Only used when it is **greater** than Max Flow Overshoot. While the compressor is at its learned floor frequency, the flow setpoint may follow up to the starting point + this value (never above the FTC's maximum flow temperature). 0 keeps the normal behaviour. Available from firmware v7.1.3 |
| Room Stop Hysteresis | `room_stop_hyst` in the CompCurve JSON, 0 to 3C | 0.5C | Ends a floor allowance run once the room temperature reaches the room setpoint + this value. Available from firmware v7.1.3 |
| Fixed Lockout Duration | `fixedlockoutduration` in the CompCurve JSON, in **minutes** | 0 (Automatic) | Use a fixed lockout time instead of the automatic 10 - 20 minutes. 0 returns to Automatic. Available from firmware v7.0.24 |

All of these settings are saved on the bridge and survive a restart.

Only the switch has a Home Assistant entity. The other settings are sent as JSON to `<base topic>/Command/System/CompCurve`, either one at a time or together:

```
{"fixedlockoutduration": 30}
```

Sets a fixed lockout of 30 minutes. Send `{"fixedlockoutduration": 0}` to go back to Automatic. In the same way, `{"max_flow_overshoot": 2}` sets the Max Flow Overshoot to 2C (this is the same as sending service code 882), and `{"floor_allowance": 6, "room_stop_hyst": 0.5}` turns on floor-aware following.

From a Home Assistant automation or script:

```yaml
action: mqtt.publish
data:
  topic: "<base topic>/Command/System/CompCurve"   # replace <base topic> with your own
  payload: '{"fixedlockoutduration": 30}'
```

## 1. Anti-stop flow following (heating)

**When it runs.** Once each time a full read of the FTC completes, while all of these are true:
* Short Cycle Protection is on
* The compressor is running in space heating, and Zone 1 is in Heating Flow mode
* The Zone 1 flow setpoint is above 0
* The post DHW letdown is not running
* No earlier write to the FTC is still waiting to be sent

**Starting point.** Following starts from the flow setpoint the Onboard Compensation Curve has calculated for Zone 1 (or the setpoint you entered, if the curve is not active). If the setpoint is already above the curve when following starts, that is used instead. While following, the starting point moves **up** if the curve rises, but never down.

**Raising and lowering the setpoint.** The bridge aims to hold the flow setpoint **1.0C below the actual flow temperature**. Logged data showed the FTC increases the compressor frequency far more often when the flow is at or below the setpoint (30+ times an hour) than when it is 1.0C above it (under once an hour), so this keeps the compressor at a low, steady frequency as well as avoiding the 2.0C stop.

| Flow temperature minus flow setpoint | Action |
|-------|------------------|
| 1.5C or more (getting close to the 2.0C stop) | **Raise**: setpoint = flow - 1.0C |
| Between 0.5C and 1.5C | **Hold**, no change |
| 0.5C or less, on **two reads in a row**, and the setpoint is above the starting point | **Lower**: setpoint = flow - 1.0C |

* The new setpoint is an absolute value, so it can move more than 0.5C in one write
* The setpoint is never lowered below the starting point, and never raised above the starting point + Max Flow Overshoot (or the floor ceiling, see below)
* A new setpoint is only written if it differs from the current one by 0.25C or more, because each write to the FTC is disruptive. The two-read wait before lowering is for the same reason
* While following is active, the Onboard Compensation Curve keeps calculating every 5 minutes but does not write its value to the FTC

**Example** (curve setpoint 35C, Max Flow Overshoot 3C, so the ceiling is 38C):

| Flow | Setpoint before | Flow - Setpoint | Action | Setpoint after |
|-------|------------------|----------------|----------------|----------------|
| 36.0 | 35.0 | 1.0 | Hold | 35.0 |
| 36.5 | 35.0 | 1.5 | Raise | 35.5 |
| 37.0 | 35.5 | 1.5 | Raise | 36.0 |
| 38.0 | 36.0 | 2.0 | Raise | 37.0 |
| 39.0 | 37.0 | 2.0 | Raise, reaches the ceiling | 38.0 |
| 39.5 | 38.0 | 1.5 | Would raise to 38.5, limited to the ceiling | 38.0 |
| 38.0 | 38.0 | 0.0 | First low read, wait | 38.0 |
| 37.5 | 38.0 | -0.5 | Second low read, lower | 36.5 |
| - | - | - | Compressor stops, hand back | 35.0 |

Once the ceiling is reached the setpoint cannot follow any further. If the flow keeps rising past ceiling + 2.0C, the FTC stops the outdoor unit as normal.

**Hand back.** When the compressor stops (not during a defrost), the bridge writes the Onboard Compensation Curve value back to the Zone 1 flow setpoint (or the setpoint you entered, if the curve is not active) and following ends. If the stop happened during a defrost, the hand back happens once the defrost has finished and the compressor is still stopped.

## Floor-aware following (Floor Allowance)

At its floor frequency the compressor cannot turn down any further, so the flow keeps rising until the emitters can pass the heat (typically a mean water temperature around 12 - 13C above the room). The normal ceiling stops the setpoint following that far, so the FTC cuts the outdoor unit. Setting Floor Allowance above Max Flow Overshoot lets the flow reach that balance point and run continuously instead.

* **At the floor:** the ceiling becomes starting point + Floor Allowance, but no higher than the FTC's maximum flow temperature (it is never lower than the normal ceiling)
* **Leaves the floor mid-run:** if the setpoint is already above the normal ceiling, it is held where it is and not pulled down (pulling it down would itself stop the outdoor unit). It just stops rising
* **Room satisfied:** once the room temperature reaches room setpoint + Room Stop Hysteresis, the run is ended. The setpoint goes back to the curve and the FTC stops the outdoor unit on its own 2.0C rule. This is held until the compressor next starts

The room temperature and setpoint come from the same place as the Onboard Compensation Curve's Room Influence. With `"use_local_tsensor": true` set for zone1, the FTC's Zone 1 temperature and setpoint are used, otherwise the `room_temperature` and `room_setpoint` values you send. Those sent values are not saved, so after a restart the room-satisfied stop only works once they have been sent again. Without them, a floor run continues until the compressor stops by itself.

**Learning the floor frequency.** The floor depends on the outdoor temperature and differs between models (some compressors reduce to 16Hz), so the bridge learns it rather than assuming it:
* It records the lowest frequency held for **5 minutes**, ignoring the first **15 minutes** of each run, during space heating only and away from defrosts
* It is learned separately for each 1C band of outdoor temperature (from the FTC's own outdoor sensor, -20C to +20C)
* A band's floor is only trusted once it has been seen in **3 separate compressor runs**. If a colder band has a lower trusted floor, the lower value is used
* The table is saved on the bridge (at most every 10 minutes while learning) and survives a restart

Until a floor is trusted for the current outdoor temperature, following behaves as if Floor Allowance was off. Service code 990 publishes the learned table and 991 clears it, see [Adapter Special Service Codes](#adapter-special-service-codes).

## 2. Anti-stop flow following (cooling)

While the compressor is running in cooling with Zone 1 in Cooling Flow mode, the setpoint is moved in fixed 0.5C steps:

| Condition | Action |
|-------|------------------|
| Flow is more than 1.0C **below** the setpoint and still falling | Setpoint lowered by 0.5C, up to Max Flow Overshoot in total (3C = 6 steps) |
| Flow is less than 0.5C below the setpoint, after it has been lowered | Setpoint raised by 0.5C (one step undone) |

The setpoint is handed back when the compressor stops, the same as for heating.

## 3. DHW flow following

While hot water is being heated the flow temperature rises far above the heating setpoint. When the FTC switched back to space heating, the flow would be more than 2.0C above the setpoint and the outdoor unit would stop. To prevent this, every **30 seconds** during hot water the bridge sets the Zone 1 flow setpoint to the actual flow temperature.

This only happens when Zone 1 is in Heating Flow mode and the Onboard Compensation Curve is active for Zone 1 or Zone 2.

## 4. Post DHW letdown

After hot water the flow temperature falls slowly, at a rate that depends on your installation. Holding the setpoint **above** the flow only keeps the compressor working towards a target it cannot reach, so instead the bridge holds the setpoint **0.5C below** the actual flow. The FTC reduces the compressor frequency and the flow comes down towards the curve, while staying inside the 2.0C stop limit. Anti-stop flow following is paused while this runs.

The bridge hands control back to the Onboard Compensation Curve as soon as the flow is within 0.5C of the curve's flow setpoint (straight away if it already is when hot water ends), or after **15 minutes** at most. If hot water starts again, DHW flow following takes over again.

The letdown only starts if DHW flow following was running during that hot water run, so a very short run (under 30 seconds) does not start it. It needs Zone 1 in Heating Flow mode and the Onboard Compensation Curve active for Zone 1. If only Zone 2's curve is active, control is handed straight back to the compensation curve when hot water ends.

## 5. Short cycle lockout

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

## What each part needs
| Part | Short Cycle Protection switch | Onboard Compensation Curve | Other conditions |
|-------|------------------|----------------|----------------|
| Anti-stop following (heating) | On | Not needed (if active, its value is the starting point) | Compressor running, Zone 1 in Heating Flow mode |
| Floor-aware following | On | Not needed | Floor Allowance > Max Flow Overshoot, floor learned for the current outdoor temperature |
| Anti-stop following (cooling) | On | Not needed | Compressor running, Zone 1 in Cooling Flow mode |
| DHW flow following | On | Zone 1 or Zone 2 active | Zone 1 in Heating Flow mode, hot water heating |
| Post DHW letdown | On | Zone 1 active | Zone 1 in Heating Flow mode, DHW flow following ran during the hot water run |
| Short cycle lockout | On to take action (detection runs regardless) | Not needed | Two compressor periods adding up to under 20 minutes |

## Reading the status
The **Short Cycle Protection State** sensor shows one of the following. If more than one applies, the one nearest the top of the list is shown:

| Value | Meaning |
|-------|------------------|
| Short Cycle Lockout Active | Space heating and cooling are prohibited while the lockout runs |
| Post DHW Flow Temperature Letdown Active | The flow setpoint is being held 0.5C below the flow after hot water |
| DHW Flow Temperature Following Active | The flow setpoint is following the flow temperature during hot water |
| Anti-Stop Flow Temperature Following Active (+1.5C) | The setpoint is currently this far above its starting point. `, at floor` is added when the floor ceiling applies |
| Inactive | Short Cycle Protection is on, nothing is running |
| Disabled | Short Cycle Protection is off |

The full set of values is published on the topic `<base topic>/Status/ActiveControl`, which you can read in MQTT Explorer:

| Field | Description |
|-------|------------------|
| ShortCycleProtectionEnabled | 1 when the switch is on, otherwise 0 |
| ShortCycleProtectionActive | The state text from the table above |
| ShortCycleReason | The reason recorded when the short cycle was detected |
| FixedShortCycleLockoutDuration | "Disabled (Automatic)", or the fixed duration in minutes |
| ShortCycleLockoutDuration | The length of the current or last lockout in **milliseconds**, back to 0 once it has been released |
| LastCompressorPeriods | The last two compressor periods in seconds, newest first (stop to stop) |
| FlowTemp_Target | The setpoint captured when anti-stop flow following started, restored at hand back if the curve is not active |
| FollowOffset | How far the setpoint is currently above its starting point (C) |
| FlowIncrementCounter | FollowOffset in 0.5C steps, rounded up. Above 0 means anti-stop flow following is active |
| LearnedFloorHz | The trusted floor frequency for the current outdoor temperature, 0 if not learned yet |
| AtFloor | 1 when the compressor is at or below the learned floor |
| FloorAllowance | The Floor Allowance setting |
| FloorRunEnded | 1 once the room-satisfied stop has ended the current floor run |

The compressor history and the lockout timer are only held in memory, so they start again after the bridge restarts. The learned floor table is kept.

# Service Codes
These depend on your outdoor unit and can be located within the [Service Manuals specific to your unit](https://www.manualslib.com/manual/795612/Mitsubishi-Electric-Par-21maa.html?page=51)

*Using MQTT Explorer or Home Assistant Automations, you can publish Service Codes to: `<base topic>/Command/System/Svc`*

*Results will be allocated to their standard topic location, or for ones not published the topic: `<base topic>/Status/AdvancedTwo` under "LastSvc": 995 and "LastSvcReply": 0*

*`<base topic>` is the Primary MQTT Base Topic from the bridge's setup page, for example `Ecodan/ASHP` or `ASHP`*

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
| 992 |   Short Cycle Protection      |  Toggles Short Cycle Protection (Active Control) On/Off, the same as the Short Cycle Protection switch. LastSvcReply shows 1 for On and 0 for Off | Home Assistant, MELPump |
| 991 |   Clear Learned Floor Frequencies      |  Clears the compressor floor frequencies learned for floor-aware flow following, so learning starts again. LastSvcReply shows 1 | Home Assistant (v7.1.3+) |
| 990 |   Publish Learned Floor Frequencies      |  LastSvcReply shows the learned floor table as `[[outdoor C, floor Hz, runs seen], ...]`, learned bands only. A band covers that temperature up to 1C above, and is trusted once seen in 3 runs. -1 means the table is too large to publish | Home Assistant (v7.1.3+) |
| 880 to 900 |   Short Cycle Protection Max. Flow. Overshoot      |  The Maximum Permitted Overshoot when running Short Cycle Protection where 880 = 0C and 900 = 20C. LastSvcReply shows the value in C | Home Assistant, MELPump |
