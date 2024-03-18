#include "PhaseStep.hpp"

namespace TrafficControl {

PhaseStep::PhaseStep(Config config)
    : initialDurationMs(config.initialDurationMs),
      executionFunction(config.executionFunction),
      remainingTimeMs(config.initialDurationMs), running(false) {}

void PhaseStep::registerFinishedListener(FinishedCallback *callback) {
  this->finishedCallback = callback;
}

void PhaseStep::start() {
  stop();
  running = true;
  executionFunction();
}

void PhaseStep::extend(int extensionTimeMs) {
  if (extensionTimeMs > remainingTimeMs)
    remainingTimeMs = extensionTimeMs;
}

void PhaseStep::update(int deltaTimeMs) {
  if (!running)
    return;

  remainingTimeMs -= deltaTimeMs;
  if (remainingTimeMs > 0)
    return;

  running = false;
  triggerFinishedCallback();
}

void PhaseStep::stop() {
  running = false;
  remainingTimeMs = initialDurationMs;
}

bool PhaseStep::isRunning() { return running; }

void PhaseStep::triggerFinishedCallback() {
  if (finishedCallback == nullptr)
    return;

  (*finishedCallback)();
}
} // namespace TrafficControl
