#include "PhaseSteps.hpp"
#include <iostream>

namespace TrafficControl {

PhaseSteps::PhaseSteps(Config config)
    : config(config), currentStepIndex(0), elapsedTime(0) {}

void PhaseSteps::start() {
  currentStepIndex = 0;
  elapsedTime = 0;

  if (currentStepIndex >= config.steps.size())
    config.onFinished();
  else
    config.steps[currentStepIndex].executionFunction();
}

void PhaseSteps::update(int deltaTimeMs) {
  elapsedTime += deltaTimeMs;

  if (elapsedTime < config.steps[currentStepIndex].durationMs)
    return;

  currentStepIndex++;
  elapsedTime = 0;

  if (currentStepIndex >= config.steps.size())
    config.onFinished();
  else
    config.steps[currentStepIndex].executionFunction();
}
} // namespace TrafficControl