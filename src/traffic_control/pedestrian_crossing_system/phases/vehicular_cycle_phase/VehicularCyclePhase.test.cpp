#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

#include "VehicularCyclePhase.hpp"

namespace {
using VehicularCyclePhase = TrafficControl::VehicularCyclePhase;
using Config = VehicularCyclePhase::Config;
using IVehicularTrafficSignalHead = TrafficControl::IVehicularTrafficSignalHead;
using State = IVehicularTrafficSignalHead::State;

class FunctionMock {
public:
  int calls = 0;
  void call() { calls++; };
};

class DeviceControlMock {
public:
  std::vector<bool> calls;
  void controlDevice(bool state) { calls.push_back(state); };
  void reset() { calls.clear(); }
};

class DeviceControlMocks {
public:
  DeviceControlMock red;
  DeviceControlMock amber;
  DeviceControlMock green;

  void reset() {
    red.reset();
    amber.reset();
    green.reset();
  }
};

// -----------------------------------------

// class VehicularTrafficSignalHeadMock : public IVehicularTrafficSignalHead {
// public:
//   VehicularTrafficSignalHeadMock(Config config)
//       : VehicularTrafficSignalHead(config) {}

//   std::vector<State> setStateCalls;
//   std::vector<int> updateCalls;

//   void setState(State state) { setStateCalls.push_back(state); }
//   void update(int deltaTimeMs) { updateCalls.push_back(deltaTimeMs); }
// };

// // =====================================================

// static VehicularTrafficSignalHead::Config
// createConfig(DeviceControlMocks &mocks, int flashingIntervalMs = 500) {
//   return {.flashingIntervalMs = flashingIntervalMs,
//           .deviceControls = {
//               .red = [&aspect = mocks.red](
//                          bool state) { aspect.controlDevice(state); },
//               .amber = [&aspect = mocks.amber](
//                            bool state) { aspect.controlDevice(state); },
//               .green = [&aspect = mocks.green](
//                            bool state) { aspect.controlDevice(state); }}};
// }

TEST_CASE("VehicularCyclePhase") {
  SECTION(".start") {
    SECTION("it set states GREEN_CONTINUOUS") {
      // FunctionMock onFinished;
      // DeviceControlMocks mocks;
      // // VehicularTrafficSignalHead vehicularSignalHead(createConfig(mocks));
      // VehicularTrafficSignalHeadMock signalHeadMock(createConfig(mocks));

      // VehicularCyclePhase phase(
      //     {.onFinished = [&onFinished]() { onFinished.call(); },
      //      .vehicularSignalHead = signalHeadMock,
      //      .timings = {.minimumRecallMs = 5000,
      //                  .amberClearanceTimeMs = 2000,
      //                  .redClearanceTimeMs = 1000}});

      // phase.start();

      // // TODO
    }
  }
}
} // namespace
