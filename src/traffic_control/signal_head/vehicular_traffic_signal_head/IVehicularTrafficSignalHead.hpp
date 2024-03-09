#pragma once
#include "../../aspect/Aspect.hpp"

namespace TrafficControl {
/**
 * @brief Represents a vehicular traffic signal head controlling traffic lights.
 *
 * The `VehicularTrafficSignalHead` class represents a signal head that controls
 * vehicular traffic lights. It manages three aspects: red, amber, and green.
 */
class IVehicularTrafficSignalHead {
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

  virtual void setState(State state) = 0;
  virtual void update(int deltaTimeMs) = 0;
};
} // namespace TrafficControl
