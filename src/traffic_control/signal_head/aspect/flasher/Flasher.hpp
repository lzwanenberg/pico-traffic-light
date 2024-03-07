#include <functional>

namespace TrafficControl {
class Flasher {
public:
  enum State { OFF = 0, ON = 1 };

  using StateChangeCallback = std::function<void(State)>;

  struct Config {
    int intervalMs;
    StateChangeCallback onStateChange;
  };

  Flasher(Config config);
  void start();
  void update(int deltaTimeMs);

private:
  Config config;
  State state;
  int elapsedTimeMsSinceLastChange;
  void toggleState();
  void changeState(State new_state);
};
} // namespace TrafficControl
