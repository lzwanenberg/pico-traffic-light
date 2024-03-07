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
class PedestrianSignalHead {
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

  /**
   * @brief Constructs a `PedestrianSignalHead` object with the given
   * configuration.
   *
   * @param config The configuration settings for the pedestrian signal head.
   */
  PedestrianSignalHead(Config config);

  /**
   * @brief Sets the state of the pedestrian signal head to the specified state.
   *
   * @param state The state to set for the pedestrian signal head.
   */
  void setState(State state);

  /**
   * @brief Updates the pedestrian signal head based on the elapsed time since
   * the last update.
   *
   * This method should be called periodically with the elapsed time since the
   * last update.
   *
   * @param deltaTimeMs The time elapsed in milliseconds since the last update.
   */
  void update(int deltaTimeMs);

private:
  struct Aspects {
    Aspect red;
    Aspect green;
  };

  Aspects aspects;
};
} // namespace TrafficControl
