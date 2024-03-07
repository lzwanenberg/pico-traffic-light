#include "../../aspect/Aspect.hpp"

namespace TrafficControl {
class PedestrianSignalHead {
public:
  enum State { RED_NON_FLASHING, GREEN_NON_FLASHING, FLASHING_GREEN };

  struct AspectControlFunctions {
    Aspect::DeviceControlFunction red;
    Aspect::DeviceControlFunction green;
  };

  struct Config {
    int flashingIntervalMs;
    AspectControlFunctions deviceControls;
  };

  struct Aspects {
    Aspect red;
    Aspect green;
  };

  PedestrianSignalHead(Config config);
  void setState(State state);
  void update(int deltaTimeMs);

private:
  Aspects aspects;
};
} // namespace TrafficControl
