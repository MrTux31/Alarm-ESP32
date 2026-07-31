# ESP32 Alarm System

An alarm system built for the ESP32 microcontroller. The repository contains the complete PlatformIO project at its root.

## Project Structure
* `include/` - Header files (.h) containing interfaces and system constants.
* `src/` - Source files (.cpp) containing the main application logic and the AlarmManager.
* `platformio.ini` - PlatformIO project configuration file.

## Setup Instructions
1. Install VS Code and the PlatformIO IDE extension.
2. Clone this repository.
3. Open the root folder in VS Code.
4. PlatformIO will automatically download the ESP32 toolchains and dependencies.

## Configuration
System constants such as the default siren duration are defined in `include/constants.h`. You can adjust these values to comply with your local regulations.
