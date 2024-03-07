#pragma once
#include <functional>

namespace TrafficControl {

/**
 * @brief Flasher class represents a simple state toggling mechanism.
 *
 * The Flasher class provides functionality to toggle between ON and OFF states
 * based on a configured interval. It also notifies an external function
 * whenever the state changes.
 */
class Flasher {
public:
  /**
   * @brief Enumeration representing the possible states of the Flasher.
   */
  enum State { OFF = 0, ON = 1 };

  using StateChangeCallback = std::function<void(State)>;

  /**
   * @brief Configuration structure for initializing the Flasher.
   *
   * The Config struct contains parameters to configure the Flasher, such as
   * the interval between state changes and the callback function to be invoked
   * when the state changes.
   */
  struct Config {
    /**< Interval between state changes in milliseconds */
    int intervalMs;

    /**< Callback function to handle state changes */
    StateChangeCallback onStateChange;
  };

  /**
   * @brief Constructor to initialize the Flasher with the provided
   * configuration.
   *
   * @param config The configuration settings for the Flasher.
   */
  Flasher(Config config);

  /**
   * @brief Resets the Flasher and starts with the State::ON.
   *
   * Calling this method resets the Flasher's state to ON and starts the
   * flashing process from the beginning.
   */
  void start();

  /**
   * @brief Updates the Flasher based on elapsed time since last update.
   *
   * This method should be called periodically with the elapsed time since
   * the last update. It will handle toggling the state based on the configured
   * interval.
   *
   * @param deltaTimeMs The time elapsed in milliseconds since the last update.
   */
  void update(int deltaTimeMs);

private:
  Config config;
  State state;
  int elapsedTimeMsSinceLastChange;
  void toggleState();
  void changeState(State new_state);
};
} // namespace TrafficControl
