# Home Assistant Parameters Detailed Explanations

# Control
| Name | Options      | Description of Function      |
|-------|------------------|----------------|
|  DHW Boost | On/Off      |  The Ecodan will perform a Normal or Eco Hot Water run at the transition of any prohibited to not prohibited, regardless of the temperature drop, this switch utilises this function to run |
|  Fast DHW Boost | On/Off      | This is the Ecodan's Forced Hot Water mode, compressor frequency and output power are maximum - compromising efficiency for speed to heat hot water at maximum speed |
|  Glycol Strength | 0%/10%/20%/30%  | The heat coefficient of water varies with the strength of glycol used, the sensor "Computed Heat Output" will use this variable to determine the coefficient of water to be used  |
|  Outdoor Unit Size (kW) | Various  | The Computed Input Power varies depending on the outdoor unit size, to determine a suitable multiplier this value is used  |
|  Zone Operation Mode | Heating Temperature (Heating Auto Adapt), Heating Flow (Heating Fixed Flow), Heating Compensation (Heating Weather Compensation), Cooling Temperature (Cooling Auto Adapt), Cooling Flow (Cooling Fixed Flow), Dry Up  | Some configurations require Zone1 & Zone2 to be fixed to the same control mode, Zone2 may not be possible to modify if this is your case |
|  Server Control Mode | On/Off  | Server Control Mode is required to control the Prohibits over this protocol, entering Server Control Mode will ignore any schedules for prohibits in the FTC. A globe symbol appears on your Main controller |
|  Flow Thermostats | On/Off  | You must be in a Flow control Mode to change flow thermostats, current temperature can be obtained from the sensors |

# Sensors
| Name | Unit      | Description of Function      |
|-------|------------------|----------------|
|   Heat Pump Input Power | kW (floor/ceiling)      | The value provided by the FTC for input power (to 0 d.p)    |
|   Heat Pump Computed Input Power | kW      | The estimated power input calculated by the bridge controller, based on the unit size set in the control section    |
|   Heat Pump Output Power | kW (floor/ceiling)      | The value provided by the FTC for output power (to 0 d.p)    |
|   Heat Pump Computed Heat Output Power | kW       | The estimated Heating power output calculated by the bridge controller, based on the glycol strngth set in the control section   |
|   Heat Pump Computed Cool Output Power | kW       | The estimated Cooling power output calculated by the bridge controller, based on the glycol strngth set in the control section  |
|   Instant CoP | n/a       | The estimated Instant CoP computed by the bridge, based on Computed Input & Output Power  |
|   "Yesterday" figures | n/a       | The energy consumed/delivered & CoP, estimated by the FTC and matching the stats on the Main Controller |