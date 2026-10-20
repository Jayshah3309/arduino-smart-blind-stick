#include "../firmware/smart_blind_stick/control.h"
#include <assert.h>
#include <iostream>
using namespace blindstick;
int main() {
  assert(distanceCm(0) == 0);
  assert(distanceCm(115) == 0);
  assert(distanceCm(116) == 2);
  assert(distanceCm(5800) == 100);
  assert(distanceCm(23200) == 400);
  assert(distanceCm(23201) == 0);
  assert(fresh(true, 500, 0));
  assert(!fresh(true, 501, 0));
  assert(!fresh(false, 10, 0));
  assert(fresh(true, 10, UINT32_MAX - 9));
  assert(!fresh(true, 600, UINT32_MAX - 9));
  assert(classify(0, true, false) == UNKNOWN);
  assert(classify(30, false, false) == NEAR);
  assert(classify(31, false, false) == CAUTION);
  assert(classify(70, false, false) == CAUTION);
  assert(classify(71, false, false) == FAR);
  assert(classify(120, false, false) == FAR);
  assert(classify(121, false, false) == CLEAR);
  assert(classify(121, true, false) == UNKNOWN);
  assert(classify(20, true, false) == NEAR);
  assert(classify(20, true, true) == HELP);
  assert(pulse(NEAR, 0) && pulse(NEAR, 79) && !pulse(NEAR, 80));
  assert(pulse(NEAR, 160));
  assert(pulse(CAUTION, 99) && !pulse(CAUTION, 100));
  assert(pulse(FAR, 79) && !pulse(FAR, 80));
  assert(pulse(UNKNOWN, 0) && !pulse(UNKNOWN, 80));
  assert(pulse(UNKNOWN, 160) && !pulse(UNKNOWN, 240));
  assert(pulse(UNKNOWN, 2000));
  assert(pulse(HELP, 199) && !pulse(HELP, 200));
  assert(!pulse(CLEAR, 0));
  std::cout << "Blind stick control tests passed\n";
}
