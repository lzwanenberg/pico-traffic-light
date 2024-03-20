#include "VehicularCyclePhase.hpp"
#include "../phase_steps/PhaseSteps.hpp"
#include <vector>

namespace TrafficControl {

using State = IVehicularTrafficSignalHead::State;

#define GREEN_STEP_INDEX 0

VehicularCyclePhase::VehicularCyclePhase(Config config)
    : steps(PhaseSteps{
          {.steps =
               std::vector<PhaseSteps::PhaseStep>{
                   {.initialDurationMs = config.timings.minimumRecallMs,
                    .executionFunction =
                        [config]() mutable {
                          config.vehicularSignalHead->setState(
                              State::GREEN_CONTINUOUS);
                        }},
                   {.initialDurationMs = config.timings.amberClearanceTimeMs,
                    .executionFunction =
                        [config]() mutable {
                          config.vehicularSignalHead->setState(
                              State::AMBER_CONTINUOUS);
                        }},
                   {.initialDurationMs = config.timings.redClearanceTimeMs,
                    .executionFunction =
                        [config]() mutable {
                          config.vehicularSignalHead->setState(
                              State::RED_CONTINUOUS);
                        }},
               }}}),
      signalHead(config.vehicularSignalHead) {}

void VehicularCyclePhase::registerFinishedListener(FinishedCallback *callback) {
  steps.registerFinishedListener(callback);
}

void VehicularCyclePhase::start() { steps.start(); }

void VehicularCyclePhase::extend(int durationMs) {
  if (steps.getCurrentStepIndex() == GREEN_STEP_INDEX)
    steps.extendCurrentStep(durationMs);
}

void VehicularCyclePhase::reset() {
  steps.stop();
  signalHead->setState(State::RED_CONTINUOUS);
}

void VehicularCyclePhase::update(int deltaTimeMs) {
  steps.update(deltaTimeMs);
  signalHead->update(deltaTimeMs);
}

} // namespace TrafficControl
