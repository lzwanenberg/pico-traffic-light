#pragma once
#include "IPushButton.hpp"
#include <functional>

namespace TrafficControl {

class PushButton : public IPushButton {
public:
  using ButtonFeedbackWriter = std::function<void(bool)>;
  using ButtonReader = std::function<bool()>;

  enum State { IDLE, REQUEST_ACCEPTED };

  struct DeviceControl {
    ButtonFeedbackWriter writeFeedback;
    ButtonReader read;
  };

  struct Config {
    DeviceControl deviceControl;
  };

  PushButton(Config config);

  void
  registerRequestReceivedListener(RequestReceivedCallback *callback) override;
  void update(int deltaTimeMs) override;
  void completeRequest() override;

private:
  DeviceControl device;
  State state;
  RequestReceivedCallback *requestReceivedCallback;
};
} // namespace TrafficControl
