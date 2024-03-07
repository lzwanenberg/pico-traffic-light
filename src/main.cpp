#include "pico_w/gpio_writer.hpp"
#include "pico_w/pico_w.hpp"
#include "traffic_control/signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"

#define YELLOW_LED_PIN 1
#define GREEN_LED_PIN 2

#define RED_TIME 8000
#define GREEN_TIME 6000
#define YELLOW_TIME 2200

using VehicularTrafficSignalHead = TrafficControl::VehicularTrafficSignalHead;
using Config = VehicularTrafficSignalHead::Config;
using State = VehicularTrafficSignalHead::State;

int main() {
  PicoW::GPIOWriter gp0(PicoW::Pin::GP0);
  PicoW::GPIOWriter gp1(PicoW::Pin::GP1);
  PicoW::GPIOWriter gp2(PicoW::Pin::GP2);

  VehicularTrafficSignalHead::Config config = {
      .flashingIntervalMs = 1000,
      .deviceControls = {
          .red =
              std::bind(&PicoW::GPIOWriter::write, &gp0, std::placeholders::_1),
          .amber =
              std::bind(&PicoW::GPIOWriter::write, &gp1, std::placeholders::_1),
          .green =
              std::bind(&PicoW::GPIOWriter::write, &gp2, std::placeholders::_1),
      }};

  VehicularTrafficSignalHead signalHead(config);

  signalHead.setState(State::AMBER_FLASHING);

  while (true) {
    signalHead.update(200);

    // TODO, actually measure delta time and account for time taken to update
    PicoW::sleep_ms(200);
  }
}
