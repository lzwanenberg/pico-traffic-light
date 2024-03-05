#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

namespace PicoW {
    bool initialize() {
        stdio_init_all();
        if (cyw43_arch_init()) {
            // Wi-Fi init failed
            return false;
        }

        return true;
    }

    // TODO: fix naming collision
    void my_sleep_ms(int ms) {
        sleep_ms(ms);
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
