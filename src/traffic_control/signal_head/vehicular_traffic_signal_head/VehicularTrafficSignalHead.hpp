#pragma once
#include "../../aspect/Aspect.hpp"

namespace TrafficControl {
/**
 * @brief Represents a vehicular traffic signal head controlling traffic lights.
 *
 * The `VehicularTrafficSignalHead` class represents a signal head that controls
 * vehicular traffic lights. It manages three aspects: red, amber, and green.
 */
class VehicularTrafficSignalHead {
public:
  /**
   * @brief Enumeration representing the states of the vehicular traffic signal
   * head.
   *
   * The `State` enum represents the possible states of the vehicular traffic
   * signal head.
   */
  enum State {
    /** Signal for continuous red light, indicating traffic must stop. */
    RED_CONTINUOUS,

    /** Signal for continuous amber light, indicating caution for traffic. */
    AMBER_CONTINUOUS,

    /** Signal for continuous green light, indicating traffic may proceed. */
    GREEN_CONTINUOUS,

    /** Signal for flashing red light, indicating special conditions for
       traffic. */
    RED_FLASHING,

    /** Signal for flashing amber light, indicating traffic may proceed with
       caution */
    AMBER_FLASHING
  };

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
  void setState(State state);
  void update(int deltaTimeMs);

private:
  struct Aspects {
    Aspect red;
    Aspect amber;
    Aspect green;
  };

  Aspects aspects;
};
} // namespace TrafficControl
