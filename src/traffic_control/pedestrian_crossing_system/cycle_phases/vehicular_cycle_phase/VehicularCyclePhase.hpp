#pragma once
#include "../../../physical_components/signal_head/vehicular_traffic_signal_head/IVehicularTrafficSignalHead.hpp"
#include "../phase_steps/PhaseSteps.hpp"
#include "IVehicularCyclePhase.hpp"
#include <iostream>

namespace TrafficControl {
class VehicularCyclePhase : public IVehicularCyclePhase {
public:
  struct Config {
    IVehicularTrafficSignalHead *vehicularSignalHead;
    struct Timings {
      int minimumRecallMs;
      int amberClearanceTimeMs;
      int redClearanceTimeMs;
    } timings;
  };

  VehicularCyclePhase(Config config);
  void registerFinishedListener(FinishedCallback *callback) override;
  void start() override;
  void extend(int durationMs) override;
  void reset() override;
  void update(int deltaTimeMs) override;

private:
  IVehicularTrafficSignalHead *signalHead;
  PhaseSteps steps;
  FinishedCallback *finishedCallback;
};
} // namespace TrafficControl
