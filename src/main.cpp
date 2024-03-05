#include <iostream>
#include <stdio.h>
#include "pico_w/gpio_writer.hpp"
#include "pico_w/pico_w.hpp"

#define YELLOW_LED_PIN 1
#define GREEN_LED_PIN 2

#define RED_TIME 8000
#define GREEN_TIME 6000
#define YELLOW_TIME 2200

int main() {
    PicoW::GPIOWriter gp0(PicoW::Pin::GP0);
    PicoW::GPIOWriter gp1(PicoW::Pin::GP1);
    PicoW::GPIOWriter gp2(PicoW::Pin::GP2);

    while (true)
    {
        gp0.write(1);
        gp1.write(0);
        gp2.write(0);
        PicoW::sleep_ms(RED_TIME);

        gp0.write(0);
        gp1.write(0);
        gp2.write(1);
        PicoW::sleep_ms(GREEN_TIME);

        gp0.write(0);
        gp1.write(1);
        gp2.write(0);
        PicoW::sleep_ms(YELLOW_TIME);
    }
}
