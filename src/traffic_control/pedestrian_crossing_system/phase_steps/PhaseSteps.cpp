#include "PhaseSteps.hpp"
#include <iostream>

namespace TrafficControl {

PhaseSteps::PhaseSteps(Config config)
    : steps(config.steps), finishedCallback(nullptr), currentStepIndex(0),
      stepFinishedHandler(std::bind(&PhaseSteps::handleStepFinished, this)) {}

void PhaseSteps::start(FinishedCallback *callback) {
  this->finishedCallback = callback;
  stop();
  if (currentStepIndex >= steps.size())
    triggerFinishedCallback();
  else
    steps[currentStepIndex].start(&stepFinishedHandler);
}

void PhaseSteps::handleStepFinished() {
  currentStepIndex++;
  if (currentStepIndex >= steps.size())
    triggerFinishedCallback();
  else
    steps[currentStepIndex].start(&stepFinishedHandler);
}

void PhaseSteps::update(int deltaTimeMs) {
  steps[currentStepIndex].update(deltaTimeMs);
}

void PhaseSteps::stop() {
  if (steps.size() > 0)
    steps[currentStepIndex].stop();

  currentStepIndex = 0;
}

void PhaseSteps::triggerFinishedCallback() {
  if (finishedCallback == nullptr)
    return;

  (*finishedCallback)();
}
} // namespace TrafficControl
