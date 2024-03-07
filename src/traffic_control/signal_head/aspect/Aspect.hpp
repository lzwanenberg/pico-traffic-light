#include <functional>

namespace TrafficControl {
class Aspect {
  using ControlFunction = std::function<void(bool)>;

  enum State { OFF = 0, ON = 1, FLASHING = 2 };

public:
  struct Config {
    ControlFunction controlFunc;
    int flashingIntervalMs;
  };

  Aspect(Config config);
  void setState(State state);
  void update(int deltaTimeMs);

private:
  Config config;
  State state;
};
} // namespace TrafficControl
