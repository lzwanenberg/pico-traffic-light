#include <iostream>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "factorial/factorial.hpp"

int main() {
    int factorial_result1 = Factorial(5);
    int factorial_result2 = Factorial(6);
    int factorial_result3 = Factorial(7);

    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(factorial_result1);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(factorial_result1);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(factorial_result2);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(factorial_result2);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(factorial_result3);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(factorial_result3);
    }
}
