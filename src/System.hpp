#pragma once
#include "traffic_control/pedestrian_crossing_system/PedestrianCrossingSystem.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/pedestrian_cycle_phase/PedestrianCyclePhase.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/vehicular_cycle_phase/VehicularCyclePhase.hpp"
#include "traffic_control/physical_components/push_button/PushButton.hpp"
#include "traffic_control/physical_components/signal_head/aspect/Aspect.hpp"
#include "traffic_control/physical_components/signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "traffic_control/physical_components/signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"
#include <functional>

using DeviceControlFunction = TrafficControl::Aspect::DeviceControlFunction;
using PushButton = TrafficControl::PushButton;

class System {
public:
  struct Config {
    int updateIntervalMs;

    struct Controller {
      std::function<void()> initialize;
      std::function<void(int)> sleepMs;
    } controller;

    struct Aspect {
      int flashingIntervalMs;
    } aspect;

    struct SignalHeads {
      struct Vehicular {
        DeviceControlFunction red;
        DeviceControlFunction amber;
        DeviceControlFunction green;
      } vehicular;
      struct Pedestrian {
        DeviceControlFunction red;
        DeviceControlFunction green;
      } pedestrian;
    } signalHeadControls;

    PushButton::DeviceControl pushButtonControl;

    struct CyclePhases {
      struct Vehicular {
        int minimumRecallMs;
        int amberClearanceTimeMs;
        int redClearanceTimeMs;
      } vehicular;
      struct Pedestrian {
        int minimumRecallMs;
        int greenFlashingClearanceTimeMs;
        int redClearanceTimeMs;
      } pedestrian;
    } cyclePhases;
  };

  System(Config config);
  void run();

private:
  int updateIntervalMs;
  Config::Controller controller;
  TrafficControl::VehicularTrafficSignalHead vehicularSignalHead;
  TrafficControl::PedestrianSignalHead pedestrianSignalHead;
  TrafficControl::PushButton pushButton;
  TrafficControl::VehicularCyclePhase vehicularCyclePhase;
  TrafficControl::PedestrianCyclePhase pedestrianCyclePhase;
  TrafficControl::PedestrianCrossingSystem pedestrianCrossingSystem;
};
