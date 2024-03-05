#include <iostream>
#include "factorial/factorial.hpp"
#include "pico_w/pico_w.hpp"

int main() {
    int factorial_result1 = Factorial(5);
    int factorial_result2 = Factorial(6);
    int factorial_result3 = Factorial(7);
    

    if (!PicoW::initialize()) {
        printf("Initialization failed");
    }

    while (true) {
        PicoW::OnboardLED::turn_on();
        PicoW::my_sleep_ms(factorial_result1);
        PicoW::OnboardLED::turn_off();
        PicoW::my_sleep_ms(factorial_result1);

        PicoW::OnboardLED::turn_on();
        PicoW::my_sleep_ms(factorial_result1);
        PicoW::OnboardLED::turn_off();
        PicoW::my_sleep_ms(factorial_result1);

        PicoW::OnboardLED::turn_on();
        PicoW::my_sleep_ms(factorial_result1);
        PicoW::OnboardLED::turn_off();
        PicoW::my_sleep_ms(factorial_result1);
        
        PicoW::OnboardLED::turn_on();
        PicoW::my_sleep_ms(factorial_result1);
        PicoW::OnboardLED::turn_off();
        PicoW::my_sleep_ms(factorial_result1);

        PicoW::OnboardLED::turn_on();
        PicoW::my_sleep_ms(factorial_result2);
        PicoW::OnboardLED::turn_off();
        PicoW::my_sleep_ms(factorial_result2);

        PicoW::OnboardLED::turn_on();
        PicoW::my_sleep_ms(factorial_result3);
        PicoW::OnboardLED::turn_off();
        PicoW::my_sleep_ms(factorial_result3);
    }
}
