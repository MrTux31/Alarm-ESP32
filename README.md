# ESP32 Alarm System

**[English](README.md) | [Français](README.fr.md)**

A smart, ESP32-based home alarm system, usable **standalone** or **connected to the internet** with full remote control via [Blynk](https://blynk.io/).

The repository contains a complete PlatformIO project at its root.

---

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Configuration](#configuration)
- [Blynk / Remote Control](#blynk--remote-control)
- [Usage Example](#usage-example)
- [Schematics](#schematics)
- [Glossary](#glossary)
- [Contributing](#contributing)
- [License](#license)
---

## Features

### Smart loop exclusion

Most basic alarms behave the same way: if a door/window is opened once, the alarm sounds and effectively disarms itself, since re-arming it would just trigger it again immediately from the same opening.

This system handles it differently. Every door, window, or motion sensor is treated as an **independent loop**:

- When a loop is triggered, the siren sounds **and that specific loop is temporarily excluded** from detection.
- The alarm immediately re-arms, ignoring only the loop that was triggered — every other loop stays active.
- If an excluded loop is physically closed again (e.g. a door is shut), it is **automatically re-included** in the set of active loops.
- If every loop ends up excluded, the alarm can no longer sound (no loop left to detect anything) and enters a **waiting state**. As soon as any loop is closed again, the system re-arms and resumes monitoring.

This ensures the alarm stays operational and keeps protecting the rest of the property even if one entry point has already been compromised.

### Remote monitoring & control (Blynk)

The alarm can optionally connect to Wi-Fi and report its status through Blynk:

- Current status: armed / disarmed / arming / triggered
- Which loop triggered the alarm
- Remote arm / disarm
- Remote manual siren activation
- Push and email notifications on intrusion

> Blynk is used out of the box, but the communication layer is decoupled (see `src/communication`), so you're free to implement another protocol (e.g. plain MQTT) instead.

---

## Project Structure

```
.
├── include/
│   └── constants.h          # Pin mapping, delays, and other system constants
├── src/
│   ├── main.cpp              # Program entry point
│   ├── AlarmManager.cpp       # Core alarm logic (arming, loop tracking, exclusion)
│   ├── devices/               # Physical hardware abstractions (loops, sirens, etc.)
│   ├── communication/         # Internet / Blynk communication layer
│   └── Indicator/             # LED status indicator handling
├── credentials_example.ini    # Template for your Wi-Fi and Blynk credentials
└── platformio.ini             # PlatformIO project configuration
```

---

## Getting Started

1. Install [VS Code](https://code.visualstudio.com/) and the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode).
2. Clone this repository.
3. Open the root folder in VS Code.
4. PlatformIO will automatically download the ESP32 toolchains and dependencies.

---

## Configuration

### System constants

System behavior is defined in [`include/constants.h`](include/constants.h): pin mapping, timing delays, siren duration, etc. Adjust these to match your wiring and local regulations.

| Constant | Description |
|---|---|
| `PIN_LOOP_1..4` | GPIO pins for each detection loop (door/window/motion sensor) |
| `PIN_LED_LOOP_1..4` | Optional per-loop status LEDs |
| `PIN_LED_ALARM_STATUS` | Global alarm status LED |
| `PIN_RELAY_SIREN` | Siren relay pin |
| `PIN_BTN_ARM` | Optional arm/disarm push button |
| `ARMING_DELAY` | Delay before the alarm becomes active after arming |
| `ENTRY_DELAY` | Grace period on the main entry loop, to allow disarming before the siren sounds |
| `DEFAULT_SIREN_DURATION_MS` | How long the siren stays on when triggered |

### Wi-Fi & Blynk credentials

Credentials are kept out of version control. To configure them:

1. Copy `credentials_example.ini` to `credentials.ini` at the project root.
2. Fill in your own values:

```ini
[env]
build_flags =
    # Your Blynk credentials
    -DBLYNK_TEMPLATE_ID="EXAMPLE"
    -DBLYNK_TEMPLATE_NAME="ExampleName"
    -DBLYNK_AUTH_TOKEN="EXAMPLE-TOKEN"

    # Your Wi-Fi network
    -DWIFI_SSID="Example-SSID"
    -DWIFI_PASS="Example-PASS"
```

---

## Blynk / Remote Control

### Datastreams

The Blynk template exposes the following virtual pins:

| Pin | Name | Type | Description |
|---|---|---|---|
| `V0` | AlarmStatus | String | Human-readable alarm status |
| `V1` | Armed / Disarmed | Integer (0/1) | Current arm state, also used to arm/disarm remotely |
| `V2` | ManualMode | Integer (0/1) | Manually trigger the siren |
| `V3` | LoopsLogs | String | Name of the loop that was last triggered |

![Blynk datastreams configuration](doc/blynk-datastreams.png)

### Notifications

An **Intrusion** event (`loop_triggered`) is configured on the template to alert you as soon as a loop is triggered:

![Events & Notifications list](doc/blynk-events-list.png)

- Sent as push notification (requires the Blynk mobile app) and by email.
- Can be sent as a **critical alert**, overriding the phone's silent mode.

![Notification recipients and critical alert settings](doc/blynk-event-notifications.png)

- A minimum interval between two notifications can be set, to avoid being spammed if a loop is triggered repeatedly.

![Notification frequency limits](doc/blynk-event-limits.png)

### Dashboard

A simple dashboard can be built on top of these datastreams, showing:

- Alarm status (armed / disarmed / triggered)
- Which loop is currently open, if any
- An arm/disarm switch
- A manual siren button

![Dashboard template using the datastreams above](doc/blynk-dashboard-template.png)

Here's what it looks like once wired to a real device, with the alarm armed and no loop open:

![Live dashboard with the alarm armed](doc/blynk-dashboard-live.png)

To receive push notifications on your phone, install the Blynk mobile app and set up the same dashboard there.

---

## Usage Example

### Standalone alarm (no internet)

If you don't want to use the provided `main.cpp` as-is, you can build your own setup directly with the library components:

```cpp
#include <devices/DetectionLoop.h>
#include <devices/Siren/RelaySiren.h>
#include <AlarmManager.h>

// Detection loops
DetectionLoop loop1(13, "Main Door", 10000); // You can add 10s entry delay to disarm before it sounds (Not mandatory)
DetectionLoop loop2(12, "Window 1");

// Siren
RelaySiren siren(18, false); // set to true if the relay is active LOW

// Group all loops together
std::vector<DetectionLoop*> loops = {&loop1, &loop2};

// Alarm manager
AlarmManager alarmManager(siren, loops);

// Arm/disarm button state
bool lastButtonState = HIGH;

void setup() {
    Serial.begin(115200);
    alarmManager.init();
    alarmManager.setArmingDelay(20000);
}

void loop() {
    alarmManager.update();

    // Arm / disarm on button press (wire up your own button)
    bool reading = digitalRead(PIN_BTN_ARM);
    if (reading == LOW && lastButtonState == HIGH) {
        if (alarmManager.getCurrentState() == AlarmManager::DISARMED) {
            alarmManager.armAlarm();
        } else {
            alarmManager.disarmAlarm();
        }
    }
    lastButtonState = reading;
}
```

### Standalone alarm with LED indicators
 
You can add local LED feedback — one LED per loop, plus a global alarm status LED — without any internet connection, using the classes from `src/Indicator`. This lets you see at a glance whether the alarm is armed, disarmed, or currently sounding an intrusion, and which specific loop was triggered:
 
```cpp
#include <devices/DetectionLoop.h>
#include <devices/Siren/RelaySiren.h>
#include <AlarmManager.h>
#include <LoopIndicator.h>
#include <AlarmIndicator.h>
#include <IndicatorManager.h>
 
// Detection loops
DetectionLoop loop1(13, "Main Door", 10000);
DetectionLoop loop2(12, "Window 1");
 
// Siren
RelaySiren siren(18, false);
 
// Group all loops together
std::vector<DetectionLoop*> loops = {&loop1, &loop2};
 
// Alarm manager
AlarmManager alarmManager(siren, loops);
 
// One LED per loop, lit up when that loop is triggered
LoopIndicator loopIndicator1(25, alarmManager);
LoopIndicator loopIndicator2(33, alarmManager);
 
// One LED reflecting the overall alarm state (armed / disarmed / triggered)
AlarmIndicator alarmIndicator(26);
 
// Manager that drives all the indicators above
IndicatorManager indicatorManager(alarmManager);
 
void setup() {
    Serial.begin(115200);
    alarmManager.init();
    alarmManager.setArmingDelay(20000);
 
    // Register each indicator with the manager
    indicatorManager.addLoopIndicator(loopIndicator1, loop1);
    indicatorManager.addLoopIndicator(loopIndicator2, loop2);
    indicatorManager.setAlarmIndicator(alarmIndicator);
}
 
void loop() {
    alarmManager.update();
    indicatorManager.update(); // Keeps every LED in sync with the current state
}
```
 
`IndicatorManager` centralizes the update calls, so you only need to call `indicatorManager.update()` in the main loop — it takes care of refreshing every registered `LoopIndicator` and the `AlarmIndicator` based on `AlarmManager`'s current state.


### Connected alarm (Wi-Fi + Blynk)
 
The connected setup builds on the exact same `AlarmManager` / `DetectionLoop` / `RelaySiren` classes shown above, and adds a few extra pieces:
 
- **`WiFiManager`** — a singleton that connects to your Wi-Fi network and keeps the connection alive.
- **`BlynkService`** — the Blynk-specific implementation of the communication layer: it knows how to read/write Blynk virtual pins and send push/email notifications. This is the piece you'd swap out to use a different protocol (e.g. MQTT).
- **`CommunicationManager`** — the glue between `AlarmManager` and a communication service (here, `BlynkService`). It's configured with a mapping of virtual pin keys and values (status, arm/disarm, manual mode, triggered loop, notification code) and keeps the remote dashboard in sync with every state change.
- **[ezTime](https://github.com/ropg/ezTime)** — keeps notification timestamps correctly adjusted for your timezone and daylight saving time, configured via a POSIX TZ string (e.g. `CET-1CEST,M3.5.0,M10.5.0/3` for France).
Since the connected setup wires together several extra pieces, the full, working version lives directly in [`src/main.cpp`](src/main.cpp) rather than being duplicated here — that way this README never goes out of sync with the actual code.
 
If you're setting up your own connected version, start from `main.cpp`, fill in `credentials.ini` as described in [Configuration](#configuration), and use the [Blynk section](#blynk--remote-control) above to recreate the template and dashboard.

---

## Schematics
The circuit schematics for the assembly will be available soon.

---

## Glossary

- **Loop**: the wiring or communication line connecting a detection element (door/window contact, motion sensor, etc.) to the central controller (here, the ESP32). Each door, window, or motion sensor is treated as one independent loop.

---

## Contributing

Issues and pull requests are welcome. This project is still a work in progress, so feedback on the alarm logic, hardware abstractions, or documentation is appreciated.

## License
This project is distributed under the **GNU General Public License v3.0 (GPLv3)** — see the [LICENSE](LICENSE) file for details.

In summary: you are free to use, modify, and redistribute this project, including for commercial purposes, provided that any distributed derivative work remains open-source under the same license (GPLv3) and the source code is made available.
