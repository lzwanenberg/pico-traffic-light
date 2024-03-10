#include "PedestrianCyclePhase.hpp"
#include "../../phase_steps/PhaseSteps.hpp"
#include <vector>

namespace TrafficControl {

using State = IPedestrianSignalHead::State;

PedestrianCyclePhase::PedestrianCyclePhase(Config config)
    : steps(PhaseSteps{
          {.steps =
               std::vector<PhaseSteps::PhaseStep>{
                   {.durationMs = config.timings.minimumRecallMs,
                    .executionFunction =
                        [config]() mutable {
                          config.pedestrianSignalHead->setState(
                              State::GREEN_CONTINUOUS);
                        }},
                   {.durationMs = config.timings.greenFlashingClearanceTimeMs,
                    .executionFunction =
                        [config]() mutable {
                          config.pedestrianSignalHead->setState(
                              State::GREEN_FLASHING);
                        }},
                   {.durationMs = config.timings.redClearanceTimeMs,
                    .executionFunction =
                        [config]() mutable {
                          config.pedestrianSignalHead->setState(
                              State::RED_CONTINUOUS);
                        }},
               },
           .onFinished = [&config]() { config.onFinished(); }}}) {}

void PedestrianCyclePhase::start() { steps.start(); }

void PedestrianCyclePhase::update(int deltaTimeMs) {
  steps.update(deltaTimeMs);
}

} // namespace TrafficControl