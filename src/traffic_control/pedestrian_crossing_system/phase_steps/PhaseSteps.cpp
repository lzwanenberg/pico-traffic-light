#include "PhaseSteps.hpp"

namespace TrafficControl {

PhaseSteps::PhaseSteps(Config config)
    : steps(config.steps), finishedCallback(nullptr), currentStepIndex(0),
      currentStepRemainingTimeMs(0), running(false) {}

void PhaseSteps::registerFinishedListener(FinishedCallback *callback) {
  this->finishedCallback = callback;
}

void PhaseSteps::start() {
  stop();
  running = true;

  if (currentStepIndex >= steps.size())
    triggerFinishedCallback();
  else {
    currentStepRemainingTimeMs = steps[currentStepIndex].initialDurationMs;
    steps[currentStepIndex].executionFunction();
  }
}

int PhaseSteps::getCurrentStepIndex() {
  return running ? currentStepIndex : -1;
}

void PhaseSteps::update(int deltaTimeMs) {
  if (!running)
    return;

  currentStepRemainingTimeMs -= deltaTimeMs;
  if (currentStepRemainingTimeMs > 0)
    return;

  currentStepIndex++;

  if (currentStepIndex >= steps.size()) {
    currentStepRemainingTimeMs = 0;
    running = false;
    triggerFinishedCallback();
  } else {
    currentStepRemainingTimeMs = steps[currentStepIndex].initialDurationMs;
    steps[currentStepIndex].executionFunction();
  }
}

void PhaseSteps::stop() {
  running = false;
  currentStepIndex = 0;
  currentStepRemainingTimeMs = 0;
}

void PhaseSteps::triggerFinishedCallback() {
  if (finishedCallback == nullptr)
    return;

  (*finishedCallback)();
}
} // namespace TrafficControl
