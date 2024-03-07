#include "Flasher.hpp"

#include <iostream>

namespace TrafficControl {
Flasher::Flasher(Config config)
    : config(config), state(State::OFF), elapsedTimeMsSinceLastChange(0) {}

void Flasher::start() { changeState(State::ON); }

void Flasher::update(int deltaTimeMs) {
  elapsedTimeMsSinceLastChange += deltaTimeMs;
  if (elapsedTimeMsSinceLastChange >= config.intervalMs)
    toggleState();
}

void Flasher::toggleState() {
  changeState(state == State::ON ? State::OFF : State::ON);
}

void Flasher::changeState(State new_state) {
  elapsedTimeMsSinceLastChange = 0;
  state = new_state;
  config.onStateChange(state);
}

} // namespace TrafficControl