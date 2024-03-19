#include "PushButton.hpp"
#include <iostream>

namespace TrafficControl {
using Config = PushButton::Config;
using State = PushButton::State;

PushButton::PushButton(Config config)
    : device(config.deviceControl), state(State::IDLE){};

void PushButton::registerRequestReceivedListener(
    RequestReceivedCallback *callback) {
  requestReceivedCallback = callback;
}

void PushButton::update(int deltaTimeMs) {
  if (state == State::REQUEST_ACCEPTED)
    return;

  bool buttonPressed = device.read();

  if (!buttonPressed)
    return;

  RequestResponse response = (*requestReceivedCallback)();
  if (response == RequestResponse::ACCEPTED) {
    device.writeFeedback(1);
    state = State::REQUEST_ACCEPTED;
  }
}

void PushButton::completeRequest() {
  state = State::IDLE;
  device.writeFeedback(0);
}

} // namespace TrafficControl
