#pragma once
#include <functional>

namespace TrafficControl {
class PhaseStep {
public:
  using ExecutionFunction = std::function<void()>;
  using FinishedCallback = std::function<void()>;

  struct Config {
    int initialDurationMs;
    ExecutionFunction executionFunction;
  };

  PhaseStep(Config config);

  void registerFinishedListener(FinishedCallback *callback);
  void start();
  void extend(int timeMs);
  void stop();
  void update(int deltaTimeMs);

private:
  int initialDurationMs;
  ExecutionFunction executionFunction;
  FinishedCallback *finishedCallback;
  int remainingTimeMs;
  bool running;
  void triggerFinishedCallback();
};
} // namespace TrafficControl