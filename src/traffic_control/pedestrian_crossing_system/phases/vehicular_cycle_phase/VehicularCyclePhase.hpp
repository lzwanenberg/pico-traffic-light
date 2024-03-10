#pragma once
#include "../../../signal_head/vehicular_traffic_signal_head/IVehicularTrafficSignalHead.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include <iostream>

namespace TrafficControl {
class VehicularCyclePhase {
public:
  using FinishedCallback = std::function<void()>;

  struct Config {
    IVehicularTrafficSignalHead *vehicularSignalHead;
    struct Timings {
      int minimumRecallMs;
      int amberClearanceTimeMs;
      int redClearanceTimeMs;
    } timings;
  };

  VehicularCyclePhase(Config config);
  void registerFinishedListener(FinishedCallback *callback);
  void start();
  void update(int deltaTimeMs);

private:
  PhaseSteps steps;
  FinishedCallback *finishedCallback;
};
} // namespace TrafficControl
