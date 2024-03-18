#pragma once
#include "../../../signal_head/pedestrian_signal_head/IPedestrianSignalHead.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include "IPedestrianCyclePhase.hpp"
#include <iostream>

namespace TrafficControl {
class PedestrianCyclePhase : public IPedestrianCyclePhase {
public:
  struct Config {
    IPedestrianSignalHead *pedestrianSignalHead;
    struct Timings {
      int minimumRecallMs;
      int greenFlashingClearanceTimeMs;
      int redClearanceTimeMs;
    } timings;
  };

  PedestrianCyclePhase(Config config);
  void registerFinishedListener(FinishedCallback *callback) override;
  void start() override;
  void reset() override;
  void update(int deltaTimeMs) override;

private:
  IPedestrianSignalHead *signalHead;
  FinishedCallback *finishedCallback;
  PhaseSteps steps;
};
} // namespace TrafficControl
