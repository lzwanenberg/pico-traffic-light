#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

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

    void sleep_ms(int ms) {
        pico_sleep_ms(ms);
    }

    namespace OnboardLED {
        void turn_on() {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        }

        void turn_off() {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        }
    }
}
