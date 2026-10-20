# Limitations and next work

## Present scope

Three ultrasonic sensors, nearest-obstacle proximity patterns, passive piezo, MOSFET-driven vibration and a local button. No directional haptics, remote SOS, GPS, GSM, water detection, battery monitoring, enclosure or foldable structure is implemented. Sequential ranging reduces ping overlap but does not establish cross-talk immunity. The firmware is bounded blocking during echo measurement.

## Next priorities

1. Compare proposed pins/parts against the original project; update firmware and diagram together.
2. Validate driver power, transducer load, sensor readings and patterns on a bench.
3. Add actual photo/video, team contributions and test evidence.
4. Measure response time, ranging errors, misses, false alerts and power.
5. Add full-loop software tests and a simulation with explicitly stated sensor/driver limitations.
6. Investigate nonblocking echo acquisition, hysteresis/median filtering and configurable thresholds without hiding urgent nearby readings.
7. Consider directional haptics, battery monitoring, mechanical design and alternative sensors only after evaluating practical requirements.
8. Remote assistance would require a modem/network, location acquisition, credentials/contact handling and delivery/error states. A local button must not be marketed as working SMS SOS.

Water, stairs and drop-offs need dedicated sensing and evaluation. GPS does not provide indoor obstacle guidance. No future feature should be labelled implemented before it is built and verified.
