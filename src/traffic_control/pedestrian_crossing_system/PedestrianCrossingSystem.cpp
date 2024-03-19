#include "PedestrianCrossingSystem.hpp"
#include <functional>

namespace TrafficControl {

using Config = PedestrianCrossingSystem::Config;

PedestrianCrossingSystem::PedestrianCrossingSystem(Config config)
    : vehicularPhase(config.vehicularPhase),
      pedestrianPhase(config.pedestrianPhase), pushButton(config.pushButton) {

  pushButton->registerRequestReceivedListener(
      new std::function<IPushButton::RequestResponse()>(
          std::bind(&PedestrianCrossingSystem::handleButtonPush, this)));

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
  pushButton->update(deltaTimeMs);
  vehicularPhase->update(deltaTimeMs);
  pedestrianPhase->update(deltaTimeMs);
}

void PedestrianCrossingSystem::handleVehicularPhaseFinished() {
  pushButton->completeRequest();
  pedestrianPhase->start();
}

void PedestrianCrossingSystem::handlePedestrianPhaseFinished() {
  vehicularPhase->start();
}

IPushButton::RequestResponse PedestrianCrossingSystem::handleButtonPush() {
  // TODO
  return IPushButton::ACCEPTED;
}
} // namespace TrafficControl
