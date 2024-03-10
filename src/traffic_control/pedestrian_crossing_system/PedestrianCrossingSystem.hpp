#include "../signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "../signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"
// #include "pedestrian_cycle_phase/PedestrianCyclePhase.hpp"
// #include "vehicular_cycle_phase/VehicularCyclePhase.hpp"

namespace TrafficControl {
class PedestrianCrossingSystem {
public:
  struct Config {
    VehicularTrafficSignalHead vehicularSignalHead;
    PedestrianSignalHead pedestrianSignalHead;
  };

  PedestrianCrossingSystem(Config config);
  void start();
  void update(int deltaTimeMs);

private:
  enum State {
    /** Only vehicular traffic is allowed to proceed and no
     * request has been received by a pedestrian to cross.
     */
    IDLE,

    /** Only vehicular traffic is allowed to proceed, but
     * a pedestrian has sent a request to cross.
     */
    PEDESTRIAN_REQUEST_RECEIVED,

    /** Vehicular traffic has been ordered to stop and
     * pedestrians are allowed to cross.
     */
    PEDESTRIAN_CROSSING_ALLOWED
  };

  Config config;
  // PedestrianCyclePhase pedestrianPhase;
  // VehicularCyclePhase vehicularPhase;
};
} // namespace TrafficControl