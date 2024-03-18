#pragma once

#include "../signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "../signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"
#include "cycle_phases/pedestrian_cycle_phase/IPedestrianCyclePhase.hpp"
#include "cycle_phases/vehicular_cycle_phase/IVehicularCyclePhase.hpp"
#include <functional>

namespace TrafficControl {
class PedestrianCrossingSystem {
public:
  struct Config {
    IVehicularCyclePhase *vehicularPhase;
    IPedestrianCyclePhase *pedestrianPhase;
  };

  PedestrianCrossingSystem(Config config);
  void start();
  void update(int deltaTimeMs);

private:
  IVehicularCyclePhase *vehicularPhase;
  IPedestrianCyclePhase *pedestrianPhase;

  std::function<void()> *vehicularPhaseFinishedHandler;
  std::function<void()> *pedestrianPhaseFinishedHandler;

  void handleVehicularPhaseFinished();
  void handlePedestrianPhaseFinished();
};
} // namespace TrafficControl