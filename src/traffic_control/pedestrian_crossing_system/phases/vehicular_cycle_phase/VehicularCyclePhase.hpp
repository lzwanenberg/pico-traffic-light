#pragma once
#include <iostream>
#include "../../../signal_head/vehicular_traffic_signal_head/IVehicularTrafficSignalHead.hpp"
#include "../../phase_steps/PhaseSteps.hpp"

namespace TrafficControl {
class VehicularCyclePhase {
public:
  using FinishedCallback = std::function<void()>;

  struct Config {
    FinishedCallback onFinished;
    IVehicularTrafficSignalHead* vehicularSignalHead;
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
