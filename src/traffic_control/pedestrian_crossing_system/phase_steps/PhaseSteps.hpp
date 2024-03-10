#pragma once
#include <functional>

namespace TrafficControl {
class PhaseSteps {
public:
  using ExecutionFunction = std::function<void()>;
  using FinishedCallback = std::function<void()>;

  struct PhaseStep {
    int durationMs;
    ExecutionFunction executionFunction;
  };

  struct Config {
    std::vector<PhaseStep> steps;
  };

  PhaseSteps(Config config);

  void registerFinishedListener(FinishedCallback *callback);
  void start();
  void update(int deltaTimeMs);

private:
  std::vector<PhaseStep> steps;
  FinishedCallback *finishedCallback;
  int currentStepIndex;
  int elapsedTime;

  void triggerFinishedCallback();
};
} // namespace TrafficControl