#include "PedestrianCyclePhase.hpp"
#include "../phase_steps/PhaseSteps.hpp"
#include <vector>
#define GREEN_STEP_INDEX 0

namespace TrafficControl {
using State = IPedestrianSignalHead::State;

PedestrianCyclePhase::PedestrianCyclePhase(Config config)
    : steps(PhaseSteps{
          {.steps =
               std::vector<PhaseSteps::PhaseStep>{
                   {.initialDurationMs = config.timings.minimumRecallMs,
                    .executionFunction =
                        [config]() mutable {
                          config.pedestrianSignalHead->setState(
                              State::GREEN_CONTINUOUS);
                        }},
                   {.initialDurationMs =
                        config.timings.greenFlashingClearanceTimeMs,
                    .executionFunction =
                        [config]() mutable {
                          config.pedestrianSignalHead->setState(
                              State::GREEN_FLASHING);
                        }},
                   {.initialDurationMs = config.timings.redClearanceTimeMs,
                    .executionFunction =
                        [config]() mutable {
                          config.pedestrianSignalHead->setState(
                              State::RED_CONTINUOUS);
                        }},
               }}}),
      signalHead(config.pedestrianSignalHead), pushButton(config.pushButton),
      requested(false) {
  pushButton->registerRequestReceivedListener(
      new std::function<IPushButton::RequestResponse()>(
          std::bind(&PedestrianCyclePhase::handleButtonPush, this)));
}

void PedestrianCyclePhase::registerFinishedListener(
    FinishedCallback *callback) {
  steps.registerFinishedListener(callback);
}

void PedestrianCyclePhase::start() {
  requested = false;
  pushButton->completeRequest();
  steps.start();
}

void PedestrianCyclePhase::reset() {
  steps.stop();
  signalHead->setState(State::RED_CONTINUOUS);
}

void PedestrianCyclePhase::update(int deltaTimeMs) {
  steps.update(deltaTimeMs);
  pushButton->update(deltaTimeMs);
  signalHead->update(deltaTimeMs);
}

bool PedestrianCyclePhase::isRequested() { return requested; }

IPushButton::RequestResponse PedestrianCyclePhase::handleButtonPush() {
  if (steps.getCurrentStepIndex() == GREEN_STEP_INDEX)
    return IPushButton::REJECTED;

  requested = true;
  return IPushButton::ACCEPTED;
}

} // namespace TrafficControl
