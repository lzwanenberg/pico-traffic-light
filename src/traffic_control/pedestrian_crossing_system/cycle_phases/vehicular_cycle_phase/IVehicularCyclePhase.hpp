#pragma once

namespace TrafficControl {
/**
 * @brief Represents a vehicular traffic signal head controlling
 * traffic lights.
 *
 * The `IVehicularCyclePhase` interface represents a signal head that controls
 * vehicular traffic lights.
 */
class IVehicularCyclePhase {
public:
  using FinishedCallback = std::function<void()>;
  virtual void start(FinishedCallback *callback) = 0;
  virtual void reset() = 0;
  virtual void update(int deltaTimeMs) = 0;
};
} // namespace TrafficControl
