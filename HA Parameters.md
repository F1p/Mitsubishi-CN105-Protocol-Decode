# Home Assistant Parameters Detailed Explanations

# Control
| Name | Options      | Description of Function      |
|-------|------------------|----------------|
|  DHW Boost | On/Off      |  The Ecodan will perform a Normal or Eco Hot Water run at the transition of any prohibited to not prohibited, regardless of the temperature drop, this switch utilises this function to run |
|  Fast DHW Boost | On/Off      | This is the Ecodan's Forced Hot Water mode, compressor frequency and output power are maximum - compromising efficiency and wear for speed to heat hot water at maximum speed |
|  Glycol Strength | 0%/10%/20%/30%  | The heat coefficient of water varies with the strength of glycol used, the sensor "Computed Heat Output" will use this variable to determine the coefficient of water to be used  |
|  Outdoor Unit Size (kW) | Various  | The Computed Input Power varies depending on the outdoor unit size, to determine a suitable multiplier this value is used  |
|  Zone Operation Mode | Heating Temperature (Heating Auto Adapt), Heating Flow (Heating Fixed Flow), Heating Compensation (Heating Weather Compensation), Cooling Temperature (Cooling Auto Adapt), Cooling Flow (Cooling Fixed Flow), Dry Up  | Some configurations require Zone1 & Zone2 to be fixed to the same control mode, Zone2 may not be possible to modify if this is your case |

# Sensors
| Value | Packet Type      | Direction      |
|-------|------------------|----------------|
|  0x41 | Set Request      | To Heat Pump   |

