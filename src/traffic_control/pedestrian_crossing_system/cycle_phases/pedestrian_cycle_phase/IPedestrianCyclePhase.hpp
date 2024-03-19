#pragma once

namespace TrafficControl {
/**
 * @brief Represents a pedestrian signal head controlling pedestrian crossing
 * lights.
 *
 * The `IPedestrianCyclePhase` interface represents a signal head that controls
 * pedestrian crossing lights.
 */
class IPedestrianCyclePhase {
public:
  using FinishedCallback = std::function<void()>;
  virtual void registerFinishedListener(FinishedCallback *callback) = 0;
  virtual void start() = 0;
  virtual void reset() = 0;
  virtual void update(int deltaTimeMs) = 0;
  virtual bool isRequested() = 0;
};
} // namespace TrafficControl
