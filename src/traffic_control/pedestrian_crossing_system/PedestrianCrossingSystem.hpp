#pragma once

#include "../push_button/IPushButton.hpp"
#include "../signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "../signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"
#include "cycle_phases/pedestrian_cycle_phase/IPedestrianCyclePhase.hpp"
#include "cycle_phases/vehicular_cycle_phase/IVehicularCyclePhase.hpp"

namespace TrafficControl {
class PedestrianCrossingSystem {
public:
  struct Config {
    IVehicularCyclePhase *vehicularPhase;
    IPedestrianCyclePhase *pedestrianPhase;
    IPushButton *pushButton;
  };

  PedestrianCrossingSystem(Config config);
  void start();
  void update(int deltaTimeMs);

private:
  IVehicularCyclePhase *vehicularPhase;
  IPedestrianCyclePhase *pedestrianPhase;
  IPushButton *pushButton;

  void handleVehicularPhaseFinished();
  void handlePedestrianPhaseFinished();
  IPushButton::RequestResponse handleButtonPush();
};
} // namespace TrafficControl