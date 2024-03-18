#include "PedestrianCrossingSystem.hpp"
#include <functional>

namespace TrafficControl {

using Config = PedestrianCrossingSystem::Config;

PedestrianCrossingSystem::PedestrianCrossingSystem(Config config)
    : vehicularPhase(config.vehicularPhase),
      pedestrianPhase(config.pedestrianPhase),
      vehicularPhaseFinishedHandler(new std::function<void()>(std::bind(
          &PedestrianCrossingSystem::handleVehicularPhaseFinished, this))),
      pedestrianPhaseFinishedHandler(new std::function<void()>(std::bind(
          &PedestrianCrossingSystem::handlePedestrianPhaseFinished, this))) {}

void PedestrianCrossingSystem::start() {
  vehicularPhase->reset();
  pedestrianPhase->reset();
  vehicularPhase->start(vehicularPhaseFinishedHandler);
}

void PedestrianCrossingSystem::update(int deltaTimeMs) {
  vehicularPhase->update(deltaTimeMs);
  pedestrianPhase->update(deltaTimeMs);
}

void PedestrianCrossingSystem::handleVehicularPhaseFinished() {
  pedestrianPhase->start(pedestrianPhaseFinishedHandler);
}

void PedestrianCrossingSystem::handlePedestrianPhaseFinished() {
  vehicularPhase->start(vehicularPhaseFinishedHandler);
}

} // namespace TrafficControl
