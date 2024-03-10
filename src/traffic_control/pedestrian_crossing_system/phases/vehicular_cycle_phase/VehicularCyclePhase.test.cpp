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
using FinishedCallback = VehicularCyclePhase::FinishedCallback;
using IVehicularTrafficSignalHead = TrafficControl::IVehicularTrafficSignalHead;
using State = IVehicularTrafficSignalHead::State;

class FinishedCallbackWrapper {
public:
  FinishedCallbackWrapper() = default;
  virtual ~FinishedCallbackWrapper() = default;

  virtual void operator()() = 0;
};

class FinishedCallbackWrapperImpl : public FinishedCallbackWrapper {
public:
  FinishedCallbackWrapperImpl(std::function<void()> func) : func_(func) {}

  void operator()() override { func_(); }

private:
  std::function<void()> func_;
};

TEST_CASE("VehicularCyclePhase") {
  SECTION(".start") {
    SECTION("it set states GREEN_CONTINUOUS") {
      auto onFinishedMock = std::make_shared<Mock<FinishedCallbackWrapper>>();

      Mock<IVehicularTrafficSignalHead> vehicularSignalHeadMock;
      IVehicularTrafficSignalHead &vehicularSignalHead =
          vehicularSignalHeadMock.get();

      Config config = {
          .onFinished =
              [onFinishedMock] { (*onFinishedMock).get().operator()(); },
          .vehicularSignalHead = &vehicularSignalHead,
          .timings = {.minimumRecallMs = 5000,
                      .amberClearanceTimeMs = 2000,
                      .redClearanceTimeMs = 1000}};
      VehicularCyclePhase phase(config);

      When(Method(*onFinishedMock, operator())).Return();
      When(Method(vehicularSignalHeadMock, setState)).Return();

      phase.start();

      Verify(Method(vehicularSignalHeadMock, setState)).Exactly(1);
      Verify(Method(*onFinishedMock, operator())).Exactly(0);
    }
  }
}
} // namespace
