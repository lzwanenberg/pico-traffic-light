#include "PedestrianCyclePhase.hpp"
#include "../../phase_steps/PhaseStep.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include <vector>

namespace TrafficControl {

using State = IPedestrianSignalHead::State;

PedestrianCyclePhase::PedestrianCyclePhase(Config config)
    : steps(PhaseSteps{
          {.steps =
               std::vector<PhaseStep>{
                   PhaseStep{
                       {.initialDurationMs = config.timings.minimumRecallMs,
                        .executionFunction =
                            [config]() mutable {
                              config.pedestrianSignalHead->setState(
                                  State::GREEN_CONTINUOUS);
                            }}},
                   PhaseStep{{.initialDurationMs =
                                  config.timings.greenFlashingClearanceTimeMs,
                              .executionFunction =
                                  [config]() mutable {
                                    config.pedestrianSignalHead->setState(
                                        State::GREEN_FLASHING);
                                  }}},
                   PhaseStep{
                       {.initialDurationMs = config.timings.redClearanceTimeMs,
                        .executionFunction =
                            [config]() mutable {
                              config.pedestrianSignalHead->setState(
                                  State::RED_CONTINUOUS);
                            }}},
               }}}),
      signalHead(config.pedestrianSignalHead) {}

void PedestrianCyclePhase::registerFinishedListener(
    FinishedCallback *callback) {
  this->finishedCallback = callback;
}

void PedestrianCyclePhase::start() { steps.start(this->finishedCallback); }

void PedestrianCyclePhase::reset() {
  steps.stop();
  signalHead->setState(State::RED_CONTINUOUS);
}

void PedestrianCyclePhase::update(int deltaTimeMs) {
  steps.update(deltaTimeMs);
  signalHead->update(deltaTimeMs);
}
} // namespace TrafficControl