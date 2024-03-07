#pragma once
#include "flasher/Flasher.hpp"
#include <functional>

namespace TrafficControl {

/**
 * @brief Represents a single lighting device on a signal head.
 *
 * The Aspect class represents an individual aspect of a signal head,
 * which is a single light that can be turned on, off, or set to a flashing
 * mode. The behavior of the Aspect is controlled by calling setState to
 * change its state, and update is used to handle the flashing loop
 * based on a configured interval.
 */
class Aspect {
public:
  /**
   * @brief Type definition for the device control function.
   *
   * The DeviceControlFunction is a function type that defines how to
   * control the physical lighting device associated with the Aspect.
   * This function is injected as a dependency during Aspect
   * initialization.
   */
  using DeviceControlFunction = std::function<void(bool)>;

  /**
   * @brief Enumeration representing the possible states of the Aspect.
   */
  enum State { OFF = 0, ON = 1, FLASHING = 2 };

  /**
   * @brief Configuration structure for initializing the Aspect.
   *
   * The Config struct contains parameters to configure the Aspect,
   * such as the flashing interval in milliseconds and the function
   * to control the physical device.
   */
  struct Config {
    /** Flashing interval in milliseconds */
    int flashingIntervalMs;

    /** Device control function */
    DeviceControlFunction deviceControlFunction;
  };

  /**
   * @brief Constructs an Aspect object with the given configuration.
   *
   * @param config The configuration settings for the Aspect.
   */
  Aspect(Config config);

  /**
   * @brief Sets the state of the Aspect to the specified state.
   *
   * This method allows setting the state of the Aspect to either
   * OFF, ON, or FLASHING. The actual behavior of the Aspect is
   * controlled by the injected device control function.
   *
   * @param state The state to set for the Aspect (OFF, ON, or FLASHING).
   */
  void setState(State state);

  /**
   * @brief Updates the Aspect based on the elapsed time since the last update.
   *
   * This method should be called periodically with the elapsed time since
   * the last update. It handles the flashing loop behavior based on the
   * configured flashing interval.
   *
   * @param deltaTimeMs The time elapsed in milliseconds since the last update.
   */
  void update(int deltaTimeMs);

private:
  Config config;
  State state;
  Flasher flasher;

  Flasher::StateChangeCallback createFlasherStateChangeHandler();
  void handleFlasherStateChange(Flasher::State state);
};
} // namespace TrafficControl
