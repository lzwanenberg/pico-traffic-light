#include "System.hpp"
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
using Pin = PicoW::Pin;

std::function<void(bool)> bind(PicoW::IWriter *writer) {
  return std::bind(&PicoW::IWriter::write, writer, std::placeholders::_1);
}

int main() {
  PicoW::initialize();

  PicoW::GPIOWriter vehicularRed(Pin::GP6);
  PicoW::GPIOWriter vehicularAmber(Pin::GP7);
  PicoW::GPIOWriter vehicularGreen(Pin::GP8);
  PicoW::GPIOWriter pedestrianRed(Pin::GP27);
  PicoW::GPIOWriter pedestrianGreen(Pin::GP26);

  System::Config config;

  config.signalHeadControls.vehicular.red = bind(&vehicularRed);
  config.signalHeadControls.vehicular.amber = bind(&vehicularAmber);
  config.signalHeadControls.vehicular.green = bind(&vehicularGreen);
  config.signalHeadControls.pedestrian.red = bind(&pedestrianRed);
  config.signalHeadControls.pedestrian.green = bind(&pedestrianGreen);

  config.aspect.flashingIntervalMs = 500;

  config.cyclePhases.pedestrian.minimumRecallMs = 8000;
  config.cyclePhases.pedestrian.greenFlashingClearanceTimeMs = 4000;
  config.cyclePhases.pedestrian.redClearanceTimeMs = 3000;
  config.cyclePhases.vehicular.minimumRecallMs = 8000;
  config.cyclePhases.vehicular.amberClearanceTimeMs = 3000;
  config.cyclePhases.vehicular.redClearanceTimeMs = 2000;

  System system(config);

  system.start();
  while (true) {
    system.update(100);

    // TODO, actually measure delta time and account for time taken to update
    PicoW::sleep_ms(100);
  }
}
