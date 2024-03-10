#include "PedestrianCrossingSystem.hpp"

namespace TrafficControl {

using State = IVehicularTrafficSignalHead::State;
using Config = PedestrianCrossingSystem::Config;

PedestrianCrossingSystem::PedestrianCrossingSystem(Config config)
    : config(config) {}

void PedestrianCrossingSystem::start() {}

void PedestrianCrossingSystem::update(int deltaTimeMs) {
  // TODO
}

} // namespace TrafficControl
