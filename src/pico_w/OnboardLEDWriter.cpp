#include "OnboardLEDWriter.hpp"
#include "pico_w.hpp"

namespace PicoW {
void OnboardLEDWriter::write(bool value) {
  if (value)
    PicoW::OnboardLED::turn_on();
  else
    PicoW::OnboardLED::turn_off();
};
} // namespace PicoW
