#pragma once
#include <functional>

namespace TrafficControl {
class IPushButton {
public:
  enum RequestResponse { ACCEPTED, REJECTED };
  using RequestReceivedCallback = std::function<RequestResponse()>;

  virtual void
  registerRequestReceivedListener(RequestReceivedCallback *callback) = 0;
  virtual void update(int deltaTimeMs) = 0;
  virtual void completeRequest() = 0;
};
} // namespace TrafficControl
