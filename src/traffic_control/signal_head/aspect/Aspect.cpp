#include "Aspect.hpp"
#include <iostream>

namespace TrafficControl {

Aspect::Aspect(Config config) : config(config) {}

void Aspect::setState(State state) {
  // Additional validation or logic can be added here
  config.controlFunc(state == State::ON);
}

void Aspect::update(int deltaTimeMs) {
  // Example update logic
  if (config.flashingIntervalMs > 0 && config.controlFunc) {
    // Perform some action based on the current state
    switch (config.flashingIntervalMs) {
    case State::OFF:
      std::cout << "Aspect is OFF\n";
      break;
    case State::ON:
      std::cout << "Aspect is ON\n";
      break;
    case State::FLASHING:
      std::cout << "Aspect is FLASHING\n";
      break;
    default:
      std::cout << "Invalid state\n";
      break;
    }
  }

  // Example usage of deltaTimeMs
  std::cout << "Updating Aspect with deltaTimeMs: " << deltaTimeMs << " ms\n";
}

} // namespace TrafficControl
