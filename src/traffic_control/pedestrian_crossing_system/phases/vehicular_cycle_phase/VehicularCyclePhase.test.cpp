#define CATCH_CONFIG_MAIN
#include "VehicularCyclePhase.hpp"
#include "../../../signal_head/vehicular_traffic_signal_head/IVehicularTrafficSignalHead.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fakeit.hpp>
#include <iostream>
#include <vector>

namespace {
using namespace fakeit;
using VehicularCyclePhase = TrafficControl::VehicularCyclePhase;
using Config = VehicularCyclePhase::Config;
using OtherConfig = VehicularCyclePhase::Config;
using IVehicularTrafficSignalHead = TrafficControl::IVehicularTrafficSignalHead;
using State = IVehicularTrafficSignalHead::State;

class FunctionMock {
public:
  int calls = 0;
  void call() { calls++; };
};

class VehicularTrafficSignalHeadMock : public IVehicularTrafficSignalHead {
public:
  int calls = 0;

  void setState(State state) override { calls++; };

  void update(int deltaTimeMs) override{
      // TODO
  };
};

TEST_CASE("VehicularCyclePhase") {
  SECTION(".start") {
    SECTION("it set states GREEN_CONTINUOUS") {
      Mock<IVehicularTrafficSignalHead> vehicularSignalHeadMock;
      When(Method(vehicularSignalHeadMock, setState)).Return();
      IVehicularTrafficSignalHead &vehicularSignalHead =
          vehicularSignalHeadMock.get();

      FunctionMock onFinished;
      // VehicularTrafficSignalHeadMock vehicularSignalHead;
      Config config = {.onFinished = [&onFinished] { onFinished.call(); },
                       .vehicularSignalHead = &vehicularSignalHead,
                       .timings = {.minimumRecallMs = 5000,
                                   .amberClearanceTimeMs = 2000,
                                   .redClearanceTimeMs = 1000}};

      VehicularCyclePhase phase(config);

      phase.start();

      Verify(Method(vehicularSignalHeadMock, setState)).Exactly(1);
    }
  }
}
} // namespace
