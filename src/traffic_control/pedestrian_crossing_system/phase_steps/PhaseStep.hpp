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

  void start(FinishedCallback *callback);
  void extend(int timeMs);
  void stop();
  void update(int deltaTimeMs);
  bool isRunning();

  int getDuration();               // TODO: temp
  ExecutionFunction getFunction(); // TODO: temp

private:
  int initialDurationMs;
  ExecutionFunction executionFunction;
  FinishedCallback *finishedCallback;
  int remainingTimeMs;
  bool running;
  void triggerFinishedCallback();
};
} // namespace TrafficControl