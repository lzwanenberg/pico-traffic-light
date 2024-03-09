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
    FinishedCallback onFinished;
  };

  PhaseSteps(Config config);
  void start();
  void update(int deltaTimeMs);

private:
  Config config;
  int currentStepIndex;
  int elapsedTime;
};
} // namespace TrafficControl