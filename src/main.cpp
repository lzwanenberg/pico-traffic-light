#include "pico_w/GPIOWriter.hpp"
#include "pico_w/OnboardLEDWriter.hpp"
#include "pico_w/PicoW.hpp"
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

int main() {
  PicoW::GPIOWriter vehicularRed(PicoW::Pin::GP6);
  PicoW::GPIOWriter vehicularAmber(PicoW::Pin::GP7);
  PicoW::GPIOWriter vehicularGreen(PicoW::Pin::GP8);

  VehicularTrafficSignalHead::Config vehicularConfig = {
      .flashingIntervalMs = 500,
      .deviceControls = {
          .red = std::bind(&PicoW::GPIOWriter::write, &vehicularRed,
                           std::placeholders::_1),
          .amber = std::bind(&PicoW::GPIOWriter::write, &vehicularAmber,
                             std::placeholders::_1),
          .green = std::bind(&PicoW::GPIOWriter::write, &vehicularGreen,
                             std::placeholders::_1),
      }};

  VehicularTrafficSignalHead vehicularSignalHead(vehicularConfig);

  PicoW::GPIOWriter pedestrianRed(PicoW::Pin::GP27);
  PicoW::GPIOWriter pedestrianGreen(PicoW::Pin::GP26);
  PedestrianSignalHead::Config pedestrianConfig = {
      .flashingIntervalMs = 500,
      .deviceControls = {
          .red = std::bind(&PicoW::GPIOWriter::write, &pedestrianRed,
                           std::placeholders::_1),
          .green = std::bind(&PicoW::GPIOWriter::write, &pedestrianGreen,
                             std::placeholders::_1),
      }};
  PedestrianSignalHead pedestrianSignalHead(pedestrianConfig);

  vehicularSignalHead.setState(
      VehicularTrafficSignalHead::State::AMBER_FLASHING);
  pedestrianSignalHead.setState(PedestrianSignalHead::State::GREEN_FLASHING);

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

  PicoW::initialize();
  PicoW::OnboardLEDWriter onboardLED;
  bool onboardLEDValue = true;
  onboardLED.write(onboardLEDValue);

  system.start();
  while (true) {
    system.update(100);

    onboardLEDValue = !onboardLEDValue;
    onboardLED.write(onboardLEDValue);

    // TODO, actually measure delta time and account for time taken to update
    PicoW::sleep_ms(100);
  }
}
