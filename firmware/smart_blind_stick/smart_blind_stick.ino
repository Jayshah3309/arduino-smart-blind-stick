/* Smart Blind Stick: NEW, UNVERIFIED reference implementation.
 * Three HC-SR04 sensors; piezo buzzer; MOSFET-driven motor; local help button.
 * This is a bench prototype, not a validated mobility aid. No GPS/GSM/SMS.
 * See hardware/wiring.md. No external Arduino libraries required.
 */
#include "control.h"
constexpr uint8_t COUNT = 3;
constexpr uint8_t TRIG[COUNT] = {2, 4, 6};
constexpr uint8_t ECHO[COUNT] = {3, 5, 7};
constexpr uint8_t BUZZER = 8; // Passive piezo, not an electromagnetic buzzer
constexpr uint8_t MOTOR_GATE = 9;
constexpr uint8_t HELP_BUTTON = 10; // Button to GND, active LOW
constexpr uint32_t PING_INTERVAL_MS = 70; // Sequential; >=60 ms between pings
constexpr uint32_t ECHO_TIMEOUT_US = 25000;
constexpr uint32_t LOG_MS = 500;
uint16_t cm[COUNT] = {};
bool valid[COUNT] = {};
uint32_t measuredAt[COUNT] = {};
uint8_t sensorIndex = 0;
uint32_t lastPing = 0;
uint32_t lastLog = 0;
bool rawButton = false;
bool stableButton = false;
uint32_t buttonChanged = 0;
blindstick::Alert current = blindstick::UNKNOWN;
uint32_t alertStarted = 0;
bool outputOn = false;

const char* alertName(blindstick::Alert a) {
  switch (a) {
    case blindstick::CLEAR: return "CLEAR";
    case blindstick::FAR: return "FAR";
    case blindstick::CAUTION: return "CAUTION";
    case blindstick::NEAR: return "NEAR";
    case blindstick::HELP: return "LOCAL_HELP";
    default: return "UNKNOWN";
  }
}
void updateButton(uint32_t now) {
  const bool pressed = digitalRead(HELP_BUTTON) == LOW;
  if (pressed != rawButton) { rawButton = pressed; buttonChanged = now; }
  if (uint32_t(now - buttonChanged) >= 30) stableButton = rawButton;
}
void setup() {
  pinMode(MOTOR_GATE, OUTPUT); digitalWrite(MOTOR_GATE, LOW);
  pinMode(BUZZER, OUTPUT); noTone(BUZZER);
  pinMode(HELP_BUTTON, INPUT_PULLUP);
  for (uint8_t i = 0; i < COUNT; ++i) {
    pinMode(TRIG[i], OUTPUT); digitalWrite(TRIG[i], LOW);
    pinMode(ECHO[i], INPUT);
  }
  Serial.begin(115200);
  Serial.println(F("UNVERIFIED bench reference; UNKNOWN is not clear path."));
  Serial.println(F("ms,front_cm,left_cm,right_cm,valid_mask,alert,output"));
}
void loop() {
  uint32_t now = millis();
  updateButton(now);
  if (uint32_t(now - lastPing) >= PING_INTERVAL_MS) {
    lastPing = now;
    digitalWrite(TRIG[sensorIndex], LOW); delayMicroseconds(2);
    digitalWrite(TRIG[sensorIndex], HIGH); delayMicroseconds(10);
    digitalWrite(TRIG[sensorIndex], LOW);
    const uint32_t echo = pulseIn(ECHO[sensorIndex], HIGH, ECHO_TIMEOUT_US);
    now = millis(); // Account for bounded blocking pulseIn call
    cm[sensorIndex] = blindstick::distanceCm(echo);
    valid[sensorIndex] = cm[sensorIndex] != 0;
    measuredAt[sensorIndex] = now;
    sensorIndex = (sensorIndex + 1) % COUNT;
    updateButton(now);
  }
  uint16_t nearest = 0;
  bool incomplete = false;
  uint8_t mask = 0;
  for (uint8_t i = 0; i < COUNT; ++i) {
    const bool ok = blindstick::fresh(valid[i], now, measuredAt[i]);
    if (ok) {
      mask |= uint8_t(1U << i);
      if (!nearest || cm[i] < nearest) nearest = cm[i];
    } else incomplete = true;
  }
  const blindstick::Alert next = blindstick::classify(nearest, incomplete, stableButton);
  if (next != current) { current = next; alertStarted = now; }
  const bool on = blindstick::pulse(current, uint32_t(now - alertStarted));
  if (on != outputOn) {
    outputOn = on;
    digitalWrite(MOTOR_GATE, on ? HIGH : LOW);
    if (on) tone(BUZZER, current == blindstick::UNKNOWN ? 1000 : 2200);
    else noTone(BUZZER);
  }
  if (uint32_t(now - lastLog) >= LOG_MS) {
    lastLog = now;
    Serial.print(now);
    for (uint8_t i = 0; i < COUNT; ++i) {
      Serial.print(','); Serial.print((mask & (1U << i)) ? cm[i] : 0);
    }
    Serial.print(','); Serial.print(mask);
    Serial.print(','); Serial.print(alertName(current));
    Serial.print(','); Serial.println(outputOn);
  }
}
