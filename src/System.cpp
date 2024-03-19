#include "System.hpp"
#include "traffic_control/aspect/Aspect.hpp"
#include "traffic_control/pedestrian_crossing_system/PedestrianCrossingSystem.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/pedestrian_cycle_phase/PedestrianCyclePhase.hpp"
#include "traffic_control/pedestrian_crossing_system/cycle_phases/vehicular_cycle_phase/VehicularCyclePhase.hpp"
#include "traffic_control/push_button/PushButton.hpp"
#include "traffic_control/signal_head/pedestrian_signal_head/PedestrianSignalHead.hpp"
#include "traffic_control/signal_head/vehicular_traffic_signal_head/VehicularTrafficSignalHead.hpp"

System::System(System::Config config)
    : vehicularSignalHead(TrafficControl::VehicularTrafficSignalHead{
          {.flashingIntervalMs = config.aspect.flashingIntervalMs,
           .deviceControls = {.red = config.signalHeadControls.vehicular.red,
                              .amber =
                                  config.signalHeadControls.vehicular.amber,
                              .green =
                                  config.signalHeadControls.vehicular.green}}}),
      pedestrianSignalHead(TrafficControl::PedestrianSignalHead{
          {.flashingIntervalMs = config.aspect.flashingIntervalMs,
           .deviceControls =
               {
                   .red = config.signalHeadControls.pedestrian.red,
                   .green = config.signalHeadControls.pedestrian.green,
               }}}),
      pushButton(TrafficControl::PushButton{
          {.deviceControl = config.pushButtonControl}}),
      vehicularCyclePhase(TrafficControl::VehicularCyclePhase{
          {.vehicularSignalHead = &vehicularSignalHead,
           .timings = {.minimumRecallMs =
                           config.cyclePhases.vehicular.minimumRecallMs,
                       .amberClearanceTimeMs =
                           config.cyclePhases.vehicular.amberClearanceTimeMs,
                       .redClearanceTimeMs =
                           config.cyclePhases.vehicular.redClearanceTimeMs}}}),
      pedestrianCyclePhase(TrafficControl::PedestrianCyclePhase{
          {.pedestrianSignalHead = &pedestrianSignalHead,
           .timings =
               {.minimumRecallMs =
                    config.cyclePhases.pedestrian.minimumRecallMs,
                .greenFlashingClearanceTimeMs =
                    config.cyclePhases.pedestrian.greenFlashingClearanceTimeMs,
                .redClearanceTimeMs =
                    config.cyclePhases.pedestrian.redClearanceTimeMs}}}),
      pedestrianCrossingSystem(TrafficControl::PedestrianCrossingSystem{
          {.vehicularPhase = &vehicularCyclePhase,
           .pedestrianPhase = &pedestrianCyclePhase,
           .pushButton = &pushButton}}) {}

void System::start() { pedestrianCrossingSystem.start(); }
void System::update(int ms) { pedestrianCrossingSystem.update(ms); }
