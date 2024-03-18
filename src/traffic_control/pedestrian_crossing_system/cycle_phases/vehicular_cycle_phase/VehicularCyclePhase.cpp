#include "VehicularCyclePhase.hpp"
#include "../../phase_steps/PhaseStep.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include <vector>

namespace TrafficControl {

using State = IVehicularTrafficSignalHead::State;

VehicularCyclePhase::VehicularCyclePhase(Config config)
    : steps(PhaseSteps{
          {.steps =
               std::vector<PhaseStep>{
                   PhaseStep{
                       {.initialDurationMs = config.timings.minimumRecallMs,
                        .executionFunction =
                            [config]() mutable {
                              config.vehicularSignalHead->setState(
                                  State::GREEN_CONTINUOUS);
                            }}},
                   PhaseStep{{.initialDurationMs =
                                  config.timings.amberClearanceTimeMs,
                              .executionFunction =
                                  [config]() mutable {
                                    config.vehicularSignalHead->setState(
                                        State::AMBER_CONTINUOUS);
                                  }}},
                   PhaseStep{
                       {.initialDurationMs = config.timings.redClearanceTimeMs,
                        .executionFunction =
                            [config]() mutable {
                              config.vehicularSignalHead->setState(
                                  State::RED_CONTINUOUS);
                            }}},
               }}}),
      signalHead(config.vehicularSignalHead) {}

void VehicularCyclePhase::start(FinishedCallback *callback) {
  steps.start(callback);
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
