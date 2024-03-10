#pragma once
#include "../../aspect/Aspect.hpp"
#include "IPedestrianSignalHead.hpp"

namespace TrafficControl {
class PedestrianSignalHead : public IPedestrianSignalHead {
public:
  /**
   * @brief Configuration structure for initializing the pedestrian signal head.
   */
  struct Config {
    /** Flashing interval in milliseconds */
    int flashingIntervalMs;

    /** Device control functions for red and green aspects */
    struct AspectControlFunctions {
      Aspect::DeviceControlFunction red;
      Aspect::DeviceControlFunction green;
    } deviceControls;
  };

  PedestrianSignalHead(Config config);
  void setState(State state);
  void update(int deltaTimeMs);

private:
  struct Aspects {
    Aspect red;
    Aspect green;
  };

  Aspects aspects;
};
} // namespace TrafficControl
