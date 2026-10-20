# Validation plan and evidence status

## Status

New reference implementation. Hardware tests, mounting evaluation, power measurements, real prototype photos and demo video have not been supplied. No claim of assistive reliability, user testing, SIH achievement or deployment is made.

Control tests exercise distance conversion, threshold boundaries, invalid readings, stale data, clock rollover, alert priority and pulse timing. GitHub Actions compiles the sketch for UNO using AVR core 1.8.6 and runs the tests. Record the verified run URL here after execution. A successful build is not hardware validation.

## Bench acceptance tests

Record date, tester, board, part models, firmware commit, IDE/core, supply, target material/angle and ruler reference. Fill observed results only after testing.

| ID | Procedure | Expected | Observed / result |
|---|---|---|---|
| B1 | Reset before sensor readings | UNKNOWN diagnostic pattern | Not run |
| B2 | One broad flat target at 20 cm | NEAR pattern, corresponding valid distance | Not run |
| B3 | Targets at 50 and 100 cm | CAUTION and FAR respectively | Not run |
| B4 | All three valid targets beyond 120 cm | CLEAR; outputs silent | Not run |
| B5 | Remove target beyond range / disconnect sensor while unpowered then retest | Invalid measurement, UNKNOWN if no nearby obstacle | Not run |
| B6 | One invalid sensor plus nearby valid obstacle | Proximity alert; valid_mask shows incomplete coverage | Not run |
| B7 | Simultaneous different-distance targets | Closest valid measurement selects pattern | Not run |
| B8 | Press/release help button | LOCAL_HELP while held; return to distance alert after release | Not run |
| B9 | Ruler checks near 30/70/120 cm thresholds | Record measured transition/error/jitter | Not run |
| B10 | Motor start/stop and 30 minute bench run | No reset, overheating or unstable sensor power | Not run |
| B11 | Soft/angled/narrow targets and adjacent sensors | Record misses, false alerts and cross-talk | Not run |
| B12 | Instrument output latency and pattern periods | Measured values reported with conditions | Not run |

Use `measurements.csv` for repeated distance readings. Compute absolute error = |measured − reference| only for valid measurements. Count invalid/missed readings separately; do not discard them from detection statistics. Report number of trials and tested target conditions rather than quoting a datasheet accuracy as your measured accuracy.

## Power and runtime

Measure UNO/sensor and motor-supply power separately and add them for total power. Include motor pulses and worst-case motor start current. Battery runtime requires a specified battery, usable energy and regulator efficiency; none has been selected. Do not infer runtime from nominal battery capacity alone.

## Assistive evaluation

Keep initial tests on a bench. Do not conduct unsupervised travel trials based on this unvalidated prototype. Any later usability work requires accessible instructions, voluntary participation, conventional mobility support and a suitable evaluation plan. This reference does not reliably detect every obstacle, drop-off or environmental hazard.
