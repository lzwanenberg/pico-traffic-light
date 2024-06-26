#include "System.hpp"
#include "pico_w/PicoW.hpp"
#include "pico_w/reader/IReader.hpp"
#include "pico_w/reader/bootsel_button_reader/BootselButtonReader.hpp"
#include "pico_w/writer/IWriter.hpp"
#include "pico_w/writer/gpio_writer/GPIOWriter.hpp"
#include "pico_w/writer/onboard_led_writer/OnboardLEDWriter.hpp"
#include "traffic_control/pedestrian_crossing_system/PedestrianCrossingSystem.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/pedestrian_cycle_phase/PedestrianCyclePhase.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/vehicular_cycle_phase/VehicularCyclePhase.hpp"
#include "traffic_control/physical_components/signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "traffic_control/physical_components/signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"

using VehicularTrafficSignalHead = TrafficControl::VehicularTrafficSignalHead;
using PedestrianSignalHead = TrafficControl::PedestrianSignalHead;
using PedestrianCrossingSystem = TrafficControl::PedestrianCrossingSystem;
using VehicularCyclePhase = TrafficControl::VehicularCyclePhase;
using PedestrianCyclePhase = TrafficControl::PedestrianCyclePhase;
using Pin = PicoW::Pin;

std::function<void(bool)> bindWriter(PicoW::IWriter *writer) {
  return std::bind(&PicoW::IWriter::write, writer, std::placeholders::_1);
}

std::function<bool()> bindReader(PicoW::IReader *reader) {
  return std::bind(&PicoW::IReader::read, reader);
}

System::Config loadConfig() {
  static PicoW::BootselButtonReader bootselButton;
  static PicoW::OnboardLEDWriter onboardLED;
  static PicoW::GPIOWriter vehicularRed(Pin::GP6);
  static PicoW::GPIOWriter vehicularAmber(Pin::GP7);
  static PicoW::GPIOWriter vehicularGreen(Pin::GP8);
  static PicoW::GPIOWriter pedestrianRed(Pin::GP27);
  static PicoW::GPIOWriter pedestrianGreen(Pin::GP26);
  static std::function<void()> initialize = []() { PicoW::initialize; };
  static std::function<void(int)> sleepMs = [](int ms) { PicoW::sleepMs(ms); };

  System::Config config;

  config.updateIntervalMs = 10;

  config.controller.initialize = initialize;
  config.controller.sleepMs = sleepMs;

  config.signalHeadControls.vehicular.red = bindWriter(&vehicularRed);
  config.signalHeadControls.vehicular.amber = bindWriter(&vehicularAmber);
  config.signalHeadControls.vehicular.green = bindWriter(&vehicularGreen);
  config.signalHeadControls.pedestrian.red = bindWriter(&pedestrianRed);
  config.signalHeadControls.pedestrian.green = bindWriter(&pedestrianGreen);

  config.pushButtonControl.read = bindReader(&bootselButton);
  config.pushButtonControl.writeFeedback = bindWriter(&onboardLED);

  config.aspect.flashingIntervalMs = 500;

  config.cyclePhases.pedestrian.minimumRecallMs = 8000;
  config.cyclePhases.pedestrian.greenFlashingClearanceTimeMs = 4000;
  config.cyclePhases.pedestrian.redClearanceTimeMs = 3000;
  config.cyclePhases.vehicular.minimumRecallMs = 12000;
  config.cyclePhases.vehicular.amberClearanceTimeMs = 3000;
  config.cyclePhases.vehicular.redClearanceTimeMs = 2000;

  return config;
}

int main() {
  PicoW::initialize();
  System::Config config = loadConfig();
  System system(config);
  system.run();
}
