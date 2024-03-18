#pragma once
#include "PhaseStep.hpp"
#include <functional>

namespace TrafficControl {
class PhaseSteps {
public:
  using ExecutionFunction = std::function<void()>;
  using FinishedCallback = std::function<void()>;

  struct Config {
    std::vector<PhaseStep> steps;
  };

  PhaseSteps(Config config);

  void start(FinishedCallback *callback);
  void stop();
  void update(int deltaTimeMs);

private:
  std::vector<PhaseStep> steps;
  PhaseStep::FinishedCallback stepFinishedHandler;
  FinishedCallback *finishedCallback;
  int currentStepIndex;

  void handleStepFinished();
  void triggerFinishedCallback();
};
} // namespace TrafficControl