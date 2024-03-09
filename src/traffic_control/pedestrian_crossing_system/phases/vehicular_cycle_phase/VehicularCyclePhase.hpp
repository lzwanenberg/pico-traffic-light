#pragma once
#include "../../../signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "../../../signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"
#include "../../phase_steps/PhaseSteps.hpp"

namespace TrafficControl {
class VehicularCyclePhase {
public:
  using FinishedCallback = std::function<void()>;

  struct Config {
    FinishedCallback onFinished;
    VehicularTrafficSignalHead vehicularSignalHead;
    struct Timings {
      int minimumRecallMs;
      int amberClearanceTimeMs;
      int redClearanceTimeMs;
    } timings;
  };

  VehicularCyclePhase(Config config);
  void start();
  void update(int deltaTimeMs);

private:
  PhaseSteps steps;
};
} // namespace TrafficControl
