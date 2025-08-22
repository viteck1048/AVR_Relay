# AVR Programmable Relay

## Overview
AVR Programmable Relay is a flexible automation device that combines the functionality of industrial programmable logic controllers (PLCs) with the simplicity and cost-effectiveness of discrete relay circuits. It's designed to modernize existing relay-based control systems without requiring complete replacement.

## Key Features
- Combines multiple relay functions in a single device
- Programmable logic for custom control scenarios
- Support for timing functions and impulse counting
- Remote monitoring and control via Wi-Fi (ESP8266-01S module)
- User-friendly interface with LCD display
- Compatible with existing relay circuits
- Non-volatile memory for settings storage
- Low power consumption

## Hardware Components
- **Main Controller**: ATmega328PB
- **Wi-Fi Module**: ESP8266-01S
- **Display**: 16x2 character LCD (1602A)
- **Inputs**: 3 opto-isolated digital inputs
- **Outputs**: 3 relay outputs with transistor drivers
- **Power Supply**: 5-25V DC (configurable)
- **Interface**: 5-button keypad for local control

## Functionality
1. **Basic Relay Functions**:
   - Standard relay operation
   - Latching relay behavior
   - AND/OR logic gates implementation

2. **Timing Functions**:
   - On-delay timer
   - Off-delay timer
   - Pulse timer
   - Cyclic operation

3. **Impulse Counter**:
   - Configurable counting modes
   - Non-volatile storage of counter values
   - Preset and current value display

4. **Network Features** (with ESP8266-01S):
   - Wi-Fi connectivity
   - Remote monitoring
   - Configuration via web interface
   - Status updates to server

## Pin Configuration
- **Inputs**:
  - IN1: PD2 (INT0)
  - IN2: PD3
  - IN3: PD4
  - IN4: PD5

- **Outputs**:
  - OUT1: PC0
  - OUT2: PC1
  - OUT3: PC2

- **Display**:
  - Data: PB0-PB3 (4-bit mode)
  - RS: PD7
  - EN: PB4
  - Backlight: PB5 (PWM controlled)

## Installation
1. Connect the hardware components as per the schematic
2. Upload the firmware using an AVR programmer
3. Configure the device settings via the LCD menu
4. (Optional) Set up Wi-Fi connection for remote access

## Usage
1. Power on the device
2. Use the navigation buttons to access the main menu
3. Select the desired function (Relay, Timer, Counter)
4. Configure the parameters using the on-screen interface
5. Save the configuration

## Configuration Menu
- **Main Menu**:
  - Relay Settings
  - Timer Settings
  - Counter Settings
  - System Settings
  - Network Configuration

## Technical Specifications
- **Operating Voltage**: 5V DC (regulated)
- **Input Voltage Range**: 5-25V DC
- **Relay Contacts**: 10A/250V AC, 10A/30V DC
- **Power Consumption**: < 2W (standby)
- **Operating Temperature**: -20°C to +60°C
- **Dimensions**: [To be specified]

## Development
### Dependencies
- Arduino IDE with AVR core
- Required libraries:
  - [LiquidCrystal](https://www.arduino.cc/en/Reference/LiquidCrystal)
  - [EEPROM](https://www.arduino.cc/en/Reference/EEPROM)
  - [ESP8266WiFi](https://github.com/esp8266/Arduino) (for network features)

### Building the Project
1. Clone the repository
2. Open the project in Arduino IDE
3. Select the appropriate board (Arduino Pro or Pro Mini, ATmega328P 3.3V/8MHz)
4. Upload the sketch

## License
[Specify your license here]

## Author
[Your Name/Organization]

## Acknowledgments
- [List any references or inspirations]

## Support
For support, please open an issue in the repository or contact [your contact information].

## Project Status
[Active/Maintenance/Mature] - [Last Updated: YYYY-MM-DD]
