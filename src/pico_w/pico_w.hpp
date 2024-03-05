namespace PicoW {
    bool initialize();
    void sleep_ms(int ms);

    namespace OnboardLED {
        void turn_on();
        void turn_off();
    }
}
