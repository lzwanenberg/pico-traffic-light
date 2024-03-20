#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

namespace PicoW {
bool initialize() {
  stdio_init_all();
  if (cyw43_arch_init()) {
    // Wi-Fi init failed
    return false;
  }

  return true;
}

void sleepMs(int ms) { sleep_ms(ms); }
} // namespace PicoW
