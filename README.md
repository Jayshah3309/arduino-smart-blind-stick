# Smart Blind Stick Using Arduino

A three-sensor Arduino UNO R3 reference prototype that converts ultrasonic distance readings into audible and vibration alert patterns.

**Status:** newly authored reference implementation; hardware verification pending. The repository was originally empty. Pins, parts and behaviour below are proposed, not recovered from the original project. This bench prototype has not been validated as a mobility aid and must not replace a conventional cane or trained assistance. Silence does not establish a safe path.

![Proposed reference wiring](hardware/circuit.svg)

## Problem and approach

A conventional cane provides tactile feedback within its physical reach. This project explores supplementary electronic obstacle alerts using front, left and right ultrasonic sensors. The controller selects the closest valid measurement and drives a passive piezo and a motor through a protected driver. Field coverage and usefulness require physical evaluation.

## Reference behaviour

| Condition | Alert | Pulse pattern (on / cycle) |
|---|---|---|
| Closest valid reading 2–30 cm | NEAR | 80 / 160 ms |
| Closest valid reading 31–70 cm | CAUTION | 100 / 400 ms |
| Closest valid reading 71–120 cm | FAR | 80 / 800 ms |
| All sensors fresh and valid, all >120 cm | CLEAR | No alert; not a safety guarantee |
| Missing/invalid/stale reading, no obstacle within 120 cm | UNKNOWN | Two 80 ms pulses every 2 seconds |
| Debounced help button held | LOCAL_HELP | 200 / 400 ms; stops on release |

Known nearby obstacles take priority over UNKNOWN. A local help alert overrides distance patterns while held. It does **not** transmit an SOS, contact anyone or provide location. One motor and one buzzer give proximity rather than directional guidance. Sensor validity remains visible in serial logs during all alert states.

## Contents

- [Firmware](firmware/smart_blind_stick/smart_blind_stick.ino) and control functions
- [Components and wiring](hardware/wiring.md)
- [Setup and bench calibration](docs/setup.md)
- [Validation procedure](docs/testing.md)
- [Measurements worksheet](docs/measurements.csv)
- [Architecture and timing](docs/architecture.md)
- [Limits and future work](docs/roadmap.md)
- [Project provenance and contributors](docs/contributors.md)
- [Photo/video instructions](media/README.md)

## Proposed components

| Component | Quantity | Specification / purpose |
|---|---:|---|
| Arduino UNO R3 | 1 | 5 V logic reference controller |
| HC-SR04, 5 V version | 3 | Front / left / right obstacle ranging |
| Passive piezo transducer | 1 | High-impedance, GPIO-compatible; no magnetic buzzer |
| Vibration motor | 1 | Rated for 5 V, stall current <=300 mA for this proposed build |
| IRLZ44N MOSFET | 1 | Low-side motor switch; verify package terminals |
| 1N5819 diode | 1 | Motor flyback protection |
| 220 Ω resistor | 1 | MOSFET gate series resistor |
| 10 kΩ resistor | 1 | Gate-to-source pulldown |
| 100 µF electrolytic + 100 nF ceramic | 1 each | Motor supply decoupling |
| Momentary pushbutton | 1 | Local help request |
| Regulated 5 V motor supply | 1 | Adequate for motor stall current; common GND |
| Breadboard, wire, USB cable | As needed | Bench assembly |

Exact motor, transducer, supply and sensor models must be confirmed. No cost or battery-life claim is made.

## Setup

1. Follow [wiring](hardware/wiring.md), including MOSFET and diode connections. Never connect a motor directly to GPIO.
2. Open `firmware/smart_blind_stick/smart_blind_stick.ino` in Arduino IDE; keep `control.h` beside it.
3. Select **Arduino AVR Boards → Arduino Uno**, choose the correct serial port, upload. No external library is needed.
4. Open Serial Monitor at **115200 baud**. Verify each sensor and alert separately on a bench using [setup](docs/setup.md).
5. Record actual results using [testing](docs/testing.md) before making performance claims.

CLI build:

```sh
arduino-cli core update-index
arduino-cli core install arduino:avr@1.8.6
arduino-cli compile --fqbn arduino:avr:uno firmware/smart_blind_stick
```

Host control tests:

```sh
g++ -std=c++11 -Wall -Wextra -Werror tests/control_test.cpp -o /tmp/blind-stick-test
/tmp/blind-stick-test
```

GitHub Actions builds for UNO and runs the host tests. Compilation and unit tests do not validate ranging, real timing, motor operation or assistive usability.

## Design limits

Sensors are triggered sequentially, 70 ms apart; each updates about every 210 ms. `pulseIn` has a 25 ms timeout and blocks while measuring, so this implementation is **not fully nonblocking**. Cross-talk, missed echoes, soft/angled objects, rain and mounting affect readings. A timeout means unknown, not zero-distance or clear. Drop-offs, stairs and head-height coverage are not reliably detected by this reference arrangement. See [architecture](docs/architecture.md).

GPS, GSM/SMS/calling, water detection, a fourth sensor, speech output, battery monitoring and foldable mechanical design are proposed extensions, not current features. No original SIH presentation or evidence has been supplied; no SIH selection, award or deployment claim is made.

## Credits and license

Repository owner: [Jayshah3309](https://github.com/Jayshah3309). Actual team roles and original-build details are pending; see [contributors](docs/contributors.md). The reference package was prepared with AI assistance. License selection is pending; [LICENSE](LICENSE) is a status notice rather than an open-source license.

## References

- [HC-SR04 datasheet](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf): 5 V supply, 10 µs trigger and suggested measurement interval >60 ms.
- [Arduino pulseIn](https://docs.arduino.cc/language-reference/en/functions/advanced-io/pulseIn/)
- [Arduino tone](https://docs.arduino.cc/language-reference/en/functions/advanced-io/tone/)
- [IRLZ44N manufacturer specifications](https://www.infineon.com/part/IRLZ44N)
- [Arduino UNO R3](https://docs.arduino.cc/hardware/uno-rev3/)
