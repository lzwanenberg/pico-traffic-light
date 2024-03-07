#include "PedestrianSignalHead.hpp"
#include "../../aspect/Aspect.hpp"

namespace TrafficControl {
PedestrianSignalHead::PedestrianSignalHead(Config config)
    : aspects(
          {.red = Aspect{{.flashingIntervalMs = config.flashingIntervalMs,
                          .deviceControlFunction = config.deviceControls.red}},
           .green = Aspect{
               {.flashingIntervalMs = config.flashingIntervalMs,
                .deviceControlFunction = config.deviceControls.green}}}) {}

void PedestrianSignalHead::setState(State state) {
  switch (state) {
  case RED_NON_FLASHING:
    aspects.red.setState(Aspect::ON);
    aspects.green.setState(Aspect::OFF);
    return;

  case GREEN_NON_FLASHING:
    aspects.red.setState(Aspect::OFF);
    aspects.green.setState(Aspect::ON);
    return;

  case FLASHING_GREEN:
    aspects.red.setState(Aspect::OFF);
    aspects.green.setState(Aspect::FLASHING);
    return;
  }
}

void PedestrianSignalHead::update(int deltaTimeMs) {
  aspects.red.update(deltaTimeMs);
  aspects.green.update(deltaTimeMs);
}
} // namespace TrafficControl
