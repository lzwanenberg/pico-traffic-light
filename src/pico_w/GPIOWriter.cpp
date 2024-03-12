
#include "GPIOWriter.hpp"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

namespace PicoW {
GPIOWriter::GPIOWriter(Pin pin) {
  pin_id = static_cast<int>(pin);
  gpio_init(pin_id);
  gpio_set_dir(pin_id, GPIO_OUT);
  gpio_pull_up(pin_id);
}

void GPIOWriter::write(bool value) { gpio_put(pin_id, value); }
} // namespace PicoW