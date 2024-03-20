#pragma once
#include "../aspect/Aspect.hpp"
#include "IVehicularTrafficSignalHead.hpp"

namespace TrafficControl {
class VehicularTrafficSignalHead : public IVehicularTrafficSignalHead {
public:
  /**
   * @brief Configuration structure for initializing the vehicular traffic
   * signal head.
   */
  struct Config {
    /** Flashing interval in milliseconds */
    int flashingIntervalMs;

    /** Device control functions for red, amber, and green aspects */
    struct AspectControlFunctions {
      Aspect::DeviceControlFunction red;
      Aspect::DeviceControlFunction amber;
      Aspect::DeviceControlFunction green;
    } deviceControls;
  };

  VehicularTrafficSignalHead(Config config);
  void setState(State state) override;
  void update(int deltaTimeMs) override;

private:
  struct Aspects {
    Aspect red;
    Aspect amber;
    Aspect green;
  };

  Aspects aspects;
};
} // namespace TrafficControl
