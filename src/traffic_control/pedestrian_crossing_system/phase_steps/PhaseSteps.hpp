#pragma once
#include <functional>

namespace TrafficControl {
class PhaseSteps {
public:
  using ExecutionFunction = std::function<void()>;
  using FinishedCallback = std::function<void()>;

  struct PhaseStep {
    int initialDurationMs;
    ExecutionFunction executionFunction;
  };

  struct Config {
    std::vector<PhaseStep> steps;
  };

  PhaseSteps(Config config);

  void registerFinishedListener(FinishedCallback *callback);
  void start();
  void stop();
  void update(int deltaTimeMs);
  int getCurrentStepIndex();
  void extendCurrentStep(int timeMs);

private:
  std::vector<PhaseStep> steps;
  FinishedCallback *finishedCallback;
  int currentStepIndex;
  int currentStepRemainingTimeMs;
  bool running;

  void triggerFinishedCallback();
};
} // namespace TrafficControl