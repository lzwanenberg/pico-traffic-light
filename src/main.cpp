#include "pico_w/PicoW.hpp"
#include "pico_w/writer/IWriter.hpp"
#include "pico_w/writer/gpio_writer/GPIOWriter.hpp"
#include "pico_w/writer/onboard_led_writer/OnboardLEDWriter.hpp"
#include "traffic_control/pedestrian_crossing_system/PedestrianCrossingSystem.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/pedestrian_cycle_phase/PedestrianCyclePhase.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/vehicular_cycle_phase/VehicularCyclePhase.hpp"
#include "traffic_control/signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "traffic_control/signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"

using VehicularTrafficSignalHead = TrafficControl::VehicularTrafficSignalHead;
using PedestrianSignalHead = TrafficControl::PedestrianSignalHead;
using PedestrianCrossingSystem = TrafficControl::PedestrianCrossingSystem;
using VehicularCyclePhase = TrafficControl::VehicularCyclePhase;
using PedestrianCyclePhase = TrafficControl::PedestrianCyclePhase;

std::function<void(bool)> bind(PicoW::IWriter *writer) {
  return std::bind(&PicoW::IWriter::write, writer, std::placeholders::_1);
}

int main() {
  PicoW::initialize();
  PicoW::OnboardLEDWriter onboardLED;

  PicoW::GPIOWriter vehicularRed(PicoW::Pin::GP6);
  PicoW::GPIOWriter vehicularAmber(PicoW::Pin::GP7);
  PicoW::GPIOWriter vehicularGreen(PicoW::Pin::GP8);

  VehicularTrafficSignalHead::Config vehicularConfig = {
      .flashingIntervalMs = 500,
      .deviceControls = {.red = bind(&vehicularRed),
                         .amber = bind(&vehicularAmber),
                         .green = bind(&vehicularGreen)}};

  VehicularTrafficSignalHead vehicularSignalHead(vehicularConfig);

  PicoW::GPIOWriter pedestrianRed(PicoW::Pin::GP27);
  PicoW::GPIOWriter pedestrianGreen(PicoW::Pin::GP26);
  PedestrianSignalHead::Config pedestrianConfig = {
      .flashingIntervalMs = 500,
      .deviceControls = {
          .red = bind(&pedestrianRed),
          .green = bind(&pedestrianGreen),
      }};

  PedestrianSignalHead pedestrianSignalHead(pedestrianConfig);

  VehicularCyclePhase::Config vehicularCycleConfig = {
      .vehicularSignalHead = &vehicularSignalHead,
      .timings = {.minimumRecallMs = 8000,
                  .amberClearanceTimeMs = 3000,
                  .redClearanceTimeMs = 2000}};

  VehicularCyclePhase vehicularCyclePhase(vehicularCycleConfig);

  PedestrianCyclePhase::Config pedestrianCycleConfig = {
      .pedestrianSignalHead = &pedestrianSignalHead,
      .timings = {.minimumRecallMs = 8000,
                  .greenFlashingClearanceTimeMs = 4000,
                  .redClearanceTimeMs = 2000}};

  PedestrianCyclePhase pedestrianCyclePhase(pedestrianCycleConfig);

  PedestrianCrossingSystem::Config systemConfig = {
      .vehicularPhase = &vehicularCyclePhase,
      .pedestrianPhase = &pedestrianCyclePhase};

  PedestrianCrossingSystem system(systemConfig);

  // bool onboardLEDValue = true;
  // onboardLED.write(onboardLEDValue);

  system.start();
  while (true) {
    system.update(100);

    // onboardLEDValue = !onboardLEDValue;
    // onboardLED.write(onboardLEDValue);

    // TODO, actually measure delta time and account for time taken to update
    PicoW::sleep_ms(100);
  }
}
