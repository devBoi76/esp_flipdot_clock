# ESP32 flipdot clock
An ESP32 clock using GPS or WiFi for the time source. It also integrates the SCD30 CO2, humidity, and temperature sensor. It will display the time on a flipdot display.

## Building
Build using Arduino IDE for the WEMOS Lolin32 OLED board.
Dependencies (libraries):
- Sensirion I2C Scd30
- Adafruit GFX
- Adafruit SSD1306

## Pinout
The pinout for the peripherals is defined in the `config.hpp` file.

## Feature details
### Time Source
#### GPS
If a GPS fix is acquired, the clock uses GPS time as the time source.
#### NTP
If the clock is connected to a WiFi network, it uses NTP time to get the UTC time.
#### Localtime
If GPS or NTP time has been acquired once, but it is no longer available, then the clock uses the ESP32's local time as the time source. This might drift over time.
#### Priority
The time sources are used in the following priority:
1. NTP
2. GPS
3. Localtime
#### Time zone
The time zone is defined in `config.hpp`. The clock ignores the NTP time zone.

### WiFi configuration
The default WiFi password and SSID can be configured in `config.hpp`.
The WiFi SSID and password can be changed over serial by typing `config`, followed by the new SSID and password. If the new connection is successful, the new SSID and password will be saved to the ESP32's flash storage.

### Flipdot Matrix
Each flipdot pixel is connected to two flipdot controllers. The contorollers pass current through a coil, which puts the pixel into the correct orientation. Because of this, the X and Y controllers have to specify opposite voltage levels, so that current can flow through the coil.

Each flipdot controller is addressed via a shift register. The input to the shift register and flipdot controller is inverted.
```
            ┌────────┐     ┌────────┐    ┌──────────┐ 
            │Inverter│     │Shift   │    │Flipdot   │ 
            │        │     │Register│    │Controller│ 
 ~Data ────>│    Data│────>│IN      │    │Rows/Cols │ 
 ~CLK  ────>│    CLK │────>│CLK     │    │(FP2800A) │ 
 ~LOAD ────>│    LOAD│────>│LATCH   │───>│ADDR      │ 
 ~EN   ────>│    EN  │──┐  │        │  ┌>│EN        │ 
            └────────┘  │  └────────┘  │ └──────────┘ 
                        │              │              
                        └──────────────┘              
```
The flipdot matrix is controlled by shifting in two 8-bit words into shift registers. The word specifies the X (or Y) address and voltage level for the controller (HIGH/DRAIN). The word format is the following:
```
 ┌───────────────────┐                                                
 │ MSB       LSB     │     Sent MSB First                             
 │  V         V      │                                                
 │ ┌─┬─┬─┬─────┐     │    ┌──────────────┐           ┌──────────────┐ 
 └─│\│D│\│ADDR │     └──> │Shift Register│ ────────> │Shift Register│ 
   └─┴─┴─┴─────┘          └──────────────┘           └──────────────┘ 
    ∧ ∧ ∧   ∧               X Coordinates              Y Coordinates  
    │ │ │   │                                                         
    │ │ │   └─── 5 bit|row/column address                             
    │ │ └─────── 1 bit|unused                                         
    │ └───────── 1 bit|Data high/low                                  
    └─────────── 1 bit|unused                                         

```

After the X and Y data is loaded into the shift registers and latched, pulling the EN pin low will enable the flipdot controllers and flip a single pixel.

> [!NOTE]
> the X and Y coordinate addresses might not be in order on some flipdot displays. Check this if your lines/columns are out of order. The mapping for the flipdot display can be found in `flipdot_interface.cpp` under `x_to_fp_addr()` and `y_to_fp_addr()`.

### SCD30 CO2 Sensor
The project uses the Sensirion SCD30 CO2 sensor to take CO2, Temperature and Humidity readings. It communicates over I2C and uses the Sensirion library.

### Photodiode
The photodiode is wired up through a comparator, where HIGH (~2.5V) indicates low light, and LOW (~0V) indicates enough light. This is used to save power and cycle counts on mechanical elements when the clock is too dark to read.

### OLED Display
Basic status information is displayed on the ESP32 dev board's built-in oled display. If no display is present, this feature will be ignored. Pins can be configured through `config.hpp`
