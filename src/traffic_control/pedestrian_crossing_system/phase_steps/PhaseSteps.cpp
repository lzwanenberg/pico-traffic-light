#include "PhaseSteps.hpp"

namespace TrafficControl {

PhaseSteps::PhaseSteps(Config config)
    : steps(config.steps), finishedCallback(nullptr), currentStepIndex(0),
      elapsedTime(0), running(false) {}

void PhaseSteps::registerFinishedListener(FinishedCallback *callback) {
  this->finishedCallback = callback;
}

void PhaseSteps::start() {
  stop();
  running = true;

  if (currentStepIndex >= steps.size())
    triggerFinishedCallback();
  else
    steps[currentStepIndex].executionFunction();
}

void PhaseSteps::update(int deltaTimeMs) {
  if (!running) // TODO: test this case
    return;

  elapsedTime += deltaTimeMs;

  if (elapsedTime < steps[currentStepIndex].durationMs)
    return;

  currentStepIndex++;
  elapsedTime = 0;

  if (currentStepIndex >= steps.size()) {
    running = false;
    triggerFinishedCallback();
  } else
    steps[currentStepIndex].executionFunction();
}

void PhaseSteps::stop() {
  running = false;
  currentStepIndex = 0;
  elapsedTime = 0;
}

void PhaseSteps::triggerFinishedCallback() {
  if (finishedCallback == nullptr)
    return;

  (*finishedCallback)();
}
} // namespace TrafficControl
