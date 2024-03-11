#include "pico_w/gpio_writer.hpp"
#include "pico_w/pico_w.hpp"
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
  PicoW::GPIOWriter gp0(PicoW::Pin::GP0);
  PicoW::GPIOWriter gp1(PicoW::Pin::GP1);
  PicoW::GPIOWriter gp2(PicoW::Pin::GP2);

  VehicularTrafficSignalHead::Config vehicularConfig = {
      .flashingIntervalMs = 500,
      .deviceControls = {
          .red =
              std::bind(&PicoW::GPIOWriter::write, &gp0, std::placeholders::_1),
          .amber =
              std::bind(&PicoW::GPIOWriter::write, &gp1, std::placeholders::_1),
          .green =
              std::bind(&PicoW::GPIOWriter::write, &gp2, std::placeholders::_1),
      }};

  VehicularTrafficSignalHead vehicularSignalHead(vehicularConfig);

  PicoW::GPIOWriter gp3(PicoW::Pin::GP3);
  PicoW::GPIOWriter gp4(PicoW::Pin::GP4);
  PedestrianSignalHead::Config pedestrianConfig = {
      .flashingIntervalMs = 500,
      .deviceControls = {
          .red =
              std::bind(&PicoW::GPIOWriter::write, &gp3, std::placeholders::_1),
          .green =
              std::bind(&PicoW::GPIOWriter::write, &gp4, std::placeholders::_1),
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

  system.start();
  while (true) {
    system.update(100);

    // TODO, actually measure delta time and account for time taken to update
    PicoW::sleep_ms(100);
  }
}
