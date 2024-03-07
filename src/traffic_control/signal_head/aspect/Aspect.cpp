#include "Aspect.hpp"
#include "flasher/Flasher.hpp"
#include <iostream>

namespace TrafficControl {
using Config = Aspect::Config;

Aspect::Aspect(Config config)
    : config(config), state(State::OFF),
      flasher({.intervalMs = config.flashingIntervalMs,
               .onStateChange = createFlasherStateChangeHandler()}) {}

void Aspect::setState(State state) {
  this->state = state;

  switch (this->state) {
  case State::ON:
    config.deviceControlFunction(true);
    return;

  case State::OFF:
    config.deviceControlFunction(false);

  case State::FLASHING:
    flasher.start();
    return;
  }
}

void Aspect::update(int deltaTimeMs) {
  if (state != State::FLASHING)
    return;

  flasher.update(deltaTimeMs);
}

Flasher::StateChangeCallback Aspect::createFlasherStateChangeHandler() {
  return std::bind(&Aspect::handleFlasherStateChange, this,
                   std::placeholders::_1);
}

void Aspect::handleFlasherStateChange(Flasher::State flasher_state) {
  if (state != State::FLASHING)
    return;

  bool value = flasher_state == Flasher::State::ON;
  config.deviceControlFunction(value);
}
} // namespace TrafficControl
