#include <iostream>
#include "factorial/factorial.hpp"
// #include "pico_w/pico_w.hpp"
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"

#define RED_LED_PIN 0
#define GREEN_LED_PIN 1

int main() {
    int sleep_time_ms = Factorial(6);

    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_pull_up(RED_LED_PIN);

    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_pull_up(GREEN_LED_PIN);

    while (true)
    {
        gpio_put(RED_LED_PIN, 1);
        gpio_put(GREEN_LED_PIN, 0);
        sleep_ms(sleep_time_ms);
        gpio_put(RED_LED_PIN, 0);
        gpio_put(GREEN_LED_PIN, 1);
        sleep_ms(sleep_time_ms);
    }
}
