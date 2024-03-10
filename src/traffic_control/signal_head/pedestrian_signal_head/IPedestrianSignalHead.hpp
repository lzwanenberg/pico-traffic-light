#pragma once
#include "../../aspect/Aspect.hpp"

namespace TrafficControl {
/**
 * @brief Represents a pedestrian signal head controlling pedestrian crossing
 * lights.
 *
 * The `PedestrianSignalHead` class represents a signal head that controls
 * pedestrian crossing lights. It manages two aspects: red and green.
 */
class IPedestrianSignalHead {
public:
  /**
   * @brief Enumeration representing the states of the pedestrian signal head.
   *
   * The `State` enum represents the possible states of the pedestrian signal
   * head, including the continuous states for pedestrians to cross.
   */
  enum State {
    /** Pedestrian signal for continuous red light. Pedestrians may not cross.
     */
    RED_CONTINUOUS,

    /** Pedestrian signal for continuous green light. Pedestrians may cross. */
    GREEN_CONTINUOUS,

    /** Pedestrian signal for flashing green light. Crossing period ending soon.
     */
    GREEN_FLASHING
  };

  /**
   * @brief Sets the state of the pedestrian signal head to the specified state.
   *
   * @param state The state to set for the pedestrian signal head.
   */
  virtual void setState(State state) = 0;

  /**
   * @brief Updates the pedestrian signal head based on the elapsed time since
   * the last update.
   *
   * This method should be called periodically with the elapsed time since the
   * last update.
   *
   * @param deltaTimeMs The time elapsed in milliseconds since the last update.
   */
  virtual void update(int deltaTimeMs) = 0;
};
} // namespace TrafficControl
