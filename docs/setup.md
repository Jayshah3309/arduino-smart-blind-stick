# Upload and bench verification

Use Arduino IDE 2.x, Arduino AVR Boards and Arduino Uno. Open the sketch in its matching folder with `control.h`. Select the actual port. Use Serial Monitor at 115200 baud. Record IDE/core versions, commit, parts and supply voltage with every test.

## Bring-up sequence

1. Disconnect motor power. Inspect sensor wiring and common GND. USB powers UNO/sensors.
2. Place a broad flat target approximately 50 cm in front of one sensor; compare serial distance against a ruler. Repeat for all three sensors. Avoid pointing them at each other.
3. Test 30, 70 and 120 cm boundaries. Values are integer centimetres; account for measurement error, not just threshold arithmetic.
4. Introduce angled, narrow and soft targets. Record misses rather than assuming the datasheet's ideal-target range applies.
5. Press and release D10's button. After 30 ms debounce the serial alert should read LOCAL_HELP while held. No remote message is sent.
6. Verify piezo pulse patterns. Then reconnect motor supply after checking MOSFET pinout, diode orientation and motor voltage/stall current. Test outputs on a bench.
7. Run the [acceptance checklist](testing.md). Never interpret silence as proof of a clear route.

## Serial columns

`ms,front_cm,left_cm,right_cm,valid_mask,alert,output`

A distance of 0 in logs means invalid/stale, not a measured zero distance. `valid_mask` is a bitmask: bit 0 front, bit 1 left, bit 2 right; 7 means all three readings are fresh and valid. A valid reading is 2–400 cm and no more than 500 ms old. No echo may mean an out-of-range object, poor reflection, disconnect or a fault; software cannot distinguish these.

## Settings

TRIG: D2/D4/D6; ECHO: D3/D5/D7. Buzzer D8, motor gate D9, button D10. Proximity thresholds are in `classify()` in `control.h`; patterns are in `pulse()`. Measurement gap is 70 ms and echo timeout 25000 µs. Change constants and documentation together and rerun tests. Per-sensor offsets, automatic calibration and EEPROM persistence are not implemented.

## Troubleshooting

| Symptom | Check |
|---|---|
| Distances invalid / UNKNOWN | Sensor VCC/GND, TRIG/ECHO swap, target reflectivity/distance |
| Implausible readings with all sensors attached | Cross-talk, supply noise and mounting |
| Motor never runs | Separate supply, shared GND, gate/source/drain, motor rating |
| UNO resets when motor starts | Supply sag, decoupling, flyback diode and ground layout |
| Piezo silent | Correct passive piezo type, D8 connection and current alert |
| Button always active | D10 short to GND or button connected across same internal contact pair |
| Upload fails | Port, UNO selection, cable and another program holding serial port |
