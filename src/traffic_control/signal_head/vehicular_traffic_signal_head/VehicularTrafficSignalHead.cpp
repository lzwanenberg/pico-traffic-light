#include "VehicularTrafficSignalHead.hpp"
#include "../../aspect/Aspect.hpp"

namespace TrafficControl {
VehicularTrafficSignalHead::VehicularTrafficSignalHead(Config config)
    : aspects(
          {.red = Aspect{{.flashingIntervalMs = config.flashingIntervalMs,
                          .deviceControlFunction = config.deviceControls.red}},
           .amber =
               Aspect{{.flashingIntervalMs = config.flashingIntervalMs,
                       .deviceControlFunction = config.deviceControls.amber}},
           .green = Aspect{
               {.flashingIntervalMs = config.flashingIntervalMs,
                .deviceControlFunction = config.deviceControls.green}}}) {
  setState(State::RED_CONTINUOUS);
}

void VehicularTrafficSignalHead::setState(State state) {
  switch (state) {
  case RED_CONTINUOUS:
    aspects.red.setState(Aspect::ON);
    aspects.amber.setState(Aspect::OFF);
    aspects.green.setState(Aspect::OFF);
    return;

  case AMBER_CONTINUOUS:
    aspects.red.setState(Aspect::OFF);
    aspects.amber.setState(Aspect::ON);
    aspects.green.setState(Aspect::OFF);
    return;

  case GREEN_CONTINUOUS:
    aspects.red.setState(Aspect::OFF);
    aspects.amber.setState(Aspect::OFF);
    aspects.green.setState(Aspect::ON);
    return;

  case RED_FLASHING:
    aspects.red.setState(Aspect::FLASHING);
    aspects.amber.setState(Aspect::OFF);
    aspects.green.setState(Aspect::OFF);
    return;

  case AMBER_FLASHING:
    aspects.red.setState(Aspect::OFF);
    aspects.amber.setState(Aspect::FLASHING);
    aspects.green.setState(Aspect::OFF);
    return;
  }
}

void VehicularTrafficSignalHead::update(int deltaTimeMs) {
  aspects.red.update(deltaTimeMs);
  aspects.amber.update(deltaTimeMs);
  aspects.green.update(deltaTimeMs);
}
} // namespace TrafficControl
