#pragma once
#include "../../../signal_head/pedestrian_signal_head/IPedestrianSignalHead.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include <iostream>

namespace TrafficControl {
class PedestrianCyclePhase {
public:
  using FinishedCallback = std::function<void()>;

  struct Config {
    FinishedCallback onFinished;
    IPedestrianSignalHead *pedestrianSignalHead;
    struct Timings {
      int minimumRecallMs;
      int greenFlashingClearanceTimeMs;
      int redClearanceTimeMs;
    } timings;
  };

  PedestrianCyclePhase(Config config);
  void start();
  void update(int deltaTimeMs);

private:
  PhaseSteps steps;
};
} // namespace TrafficControl
