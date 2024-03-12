#include "PedestrianCrossingSystem.hpp"
#include <functional>

namespace TrafficControl {

using Config = PedestrianCrossingSystem::Config;

PedestrianCrossingSystem::PedestrianCrossingSystem(Config config)
    : vehicularPhase(config.vehicularPhase),
      pedestrianPhase(config.pedestrianPhase) {

  vehicularPhase->registerFinishedListener(new std::function<void()>(std::bind(
      &PedestrianCrossingSystem::handleVehicularPhaseFinished, this)));

  pedestrianPhase->registerFinishedListener(new std::function<void()>(std::bind(
      &PedestrianCrossingSystem::handlePedestrianPhaseFinished, this)));
}

void PedestrianCrossingSystem::start() {
  vehicularPhase->reset();
  pedestrianPhase->reset();
  vehicularPhase->start();
}

void PedestrianCrossingSystem::update(int deltaTimeMs) {
  vehicularPhase->update(deltaTimeMs);
  pedestrianPhase->update(deltaTimeMs);
}

void PedestrianCrossingSystem::handleVehicularPhaseFinished() {
  pedestrianPhase->start();
}

void PedestrianCrossingSystem::handlePedestrianPhaseFinished() {
  vehicularPhase->start();
}

} // namespace TrafficControl
