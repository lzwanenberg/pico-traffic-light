#include "VehicularCyclePhase.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include <vector>

namespace TrafficControl {

using State = IVehicularTrafficSignalHead::State;

VehicularCyclePhase::VehicularCyclePhase(Config config)
    : steps(PhaseSteps{{.steps = std::vector<PhaseSteps::PhaseStep>{
                            {.durationMs = config.timings.minimumRecallMs,
                             .executionFunction =
                                 [config]() mutable {
                                   config.vehicularSignalHead->setState(
                                       State::GREEN_CONTINUOUS);
                                 }},
                            {.durationMs = config.timings.amberClearanceTimeMs,
                             .executionFunction =
                                 [config]() mutable {
                                   config.vehicularSignalHead->setState(
                                       State::AMBER_CONTINUOUS);
                                 }},
                            {.durationMs = config.timings.redClearanceTimeMs,
                             .executionFunction =
                                 [config]() mutable {
                                   config.vehicularSignalHead->setState(
                                       State::RED_CONTINUOUS);
                                 }},
                        }}}) {
  steps.registerFinishedListener(&config.onFinished);
}

void VehicularCyclePhase::start() { steps.start(); }

void VehicularCyclePhase::update(int deltaTimeMs) { steps.update(deltaTimeMs); }

} // namespace TrafficControl
