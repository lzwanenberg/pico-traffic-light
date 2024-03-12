#pragma once
#include "traffic_control/aspect/Aspect.hpp"
#include "traffic_control/pedestrian_crossing_system/PedestrianCrossingSystem.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/pedestrian_cycle_phase/PedestrianCyclePhase.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/vehicular_cycle_phase/VehicularCyclePhase.hpp"
#include "traffic_control/signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "traffic_control/signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"

using DeviceControlFunction = TrafficControl::Aspect::DeviceControlFunction;

class System {
public:
  struct Config {
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
  void start();
  void update(int deltaTimeMs);

private:
  TrafficControl::VehicularTrafficSignalHead vehicularSignalHead;
  TrafficControl::PedestrianSignalHead pedestrianSignalHead;
  TrafficControl::VehicularCyclePhase vehicularCyclePhase;
  TrafficControl::PedestrianCyclePhase pedestrianCyclePhase;
  TrafficControl::PedestrianCrossingSystem pedestrianCrossingSystem;
};
