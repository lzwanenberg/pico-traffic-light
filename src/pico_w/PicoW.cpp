#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

const auto pico_sleep_ms = sleep_ms;

namespace PicoW {
bool initialize() {
  stdio_init_all();
  if (cyw43_arch_init()) {
    // Wi-Fi init failed
    return false;
  }

  return true;
}

void sleep_ms(int ms) { pico_sleep_ms(ms); }
} // namespace PicoW
