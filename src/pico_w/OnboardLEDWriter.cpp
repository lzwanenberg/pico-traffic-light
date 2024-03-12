#include "OnboardLEDWriter.hpp"
#include "PicoW.hpp"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

namespace PicoW {
void OnboardLEDWriter::write(bool value) {
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, value);
};
} // namespace PicoW
