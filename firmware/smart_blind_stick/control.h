#pragma once
#include <stdint.h>
namespace blindstick {
enum Alert : uint8_t { CLEAR, FAR, CAUTION, NEAR, UNKNOWN, HELP };
inline bool fresh(bool valid, uint32_t now, uint32_t measuredAt) {
  return valid && uint32_t(now - measuredAt) <= 500;
}
inline uint16_t distanceCm(uint32_t echoUs) {
  if (echoUs < 116 || echoUs > 23200) return 0;
  return uint16_t(echoUs / 58);
}
inline Alert classify(uint16_t nearest, bool incomplete, bool help) {
  if (help) return HELP;
  if (nearest && nearest <= 30) return NEAR;
  if (nearest && nearest <= 70) return CAUTION;
  if (nearest && nearest <= 120) return FAR;
  return incomplete ? UNKNOWN : CLEAR;
}
inline bool pulse(Alert alert, uint32_t elapsed) {
  switch (alert) {
    case HELP: return elapsed % 400 < 200;
    case NEAR: return elapsed % 160 < 80;
    case CAUTION: return elapsed % 400 < 100;
    case FAR: return elapsed % 800 < 80;
    case UNKNOWN: return elapsed % 2000 < 80 ||
                         (elapsed % 2000 >= 160 && elapsed % 2000 < 240);
    default: return false;
  }
}
} // namespace blindstick
