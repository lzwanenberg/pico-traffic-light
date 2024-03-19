#pragma once
#include "../../../push_button/IPushButton.hpp"
#include "../../../signal_head/pedestrian_signal_head/IPedestrianSignalHead.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include "IPedestrianCyclePhase.hpp"
#include <iostream>

namespace TrafficControl {
class PedestrianCyclePhase : public IPedestrianCyclePhase {
public:
  struct Config {
    IPedestrianSignalHead *pedestrianSignalHead;
    IPushButton *pushButton;

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
  bool isRequested() override;
  IPushButton::RequestResponse handleButtonPush();

private:
  bool requested;
  IPedestrianSignalHead *signalHead;
  IPushButton *pushButton;
  PhaseSteps steps;
};
} // namespace TrafficControl
