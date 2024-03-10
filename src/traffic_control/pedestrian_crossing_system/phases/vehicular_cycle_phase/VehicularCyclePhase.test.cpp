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
using Timings = Config::Timings;

class FinishedCallbackWrapper {
public:
  FinishedCallbackWrapper() = default;
  virtual ~FinishedCallbackWrapper() = default;
  virtual void operator()() = 0;
};

struct TestContext {
  Config config;
  std::shared_ptr<Mock<FinishedCallbackWrapper>> onFinishedMock;
  Mock<IVehicularTrafficSignalHead> vehicularSignalHeadMock;
};

void initializeTestContext(TestContext &context,
                           Timings timings = {.minimumRecallMs = 3000,
                                              .amberClearanceTimeMs = 2000,
                                              .redClearanceTimeMs = 1000}) {
  context.onFinishedMock = std::make_shared<Mock<FinishedCallbackWrapper>>();
  IVehicularTrafficSignalHead &vehicularSignalHead =
      context.vehicularSignalHeadMock.get();

  context.config = {
      .onFinished =
          [&context] { (*context.onFinishedMock).get().operator()(); },
      .vehicularSignalHead = &vehicularSignalHead,
      .timings = timings};

  When(Method(*context.onFinishedMock, operator())).AlwaysReturn();
  When(Method(context.vehicularSignalHeadMock, setState)).AlwaysReturn();
}

void resetMocks(TestContext &context) {
  Fake(Method(context.vehicularSignalHeadMock, setState));
  Fake(Method(*context.onFinishedMock, operator()));
}

TEST_CASE("VehicularCyclePhase") {
  SECTION(".start") {
    SECTION("it set states GREEN_CONTINUOUS") {
      TestContext context;
      initializeTestContext(context);
      VehicularCyclePhase phase(context.config);

      phase.start();

      Verify(Method(context.vehicularSignalHeadMock, setState)
                 .Using(State::GREEN_CONTINUOUS))
          .Exactly(1);
      Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
    }
  }

  SECTION(".update") {
    SECTION("when not enough time has passed") {
      SECTION("it does not change state") {
        Timings timings = {.minimumRecallMs = 3000,
                           .amberClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        VehicularCyclePhase phase(context.config);

        phase.start();
        phase.update(1000);
        phase.update(1999);

        Verify(Method(context.vehicularSignalHeadMock, setState)).Exactly(1);
        Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
      }
    }

    SECTION("when enough time has passed") {
      SECTION("it eventually changes state to AMBER_CONTINUOUS") {
        Timings timings = {.minimumRecallMs = 3000,
                           .amberClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        VehicularCyclePhase phase(context.config);

        phase.start();
        phase.update(1000);
        phase.update(1999);
        phase.update(1); // 3000 ms passed (minimumRecallMs)

        Verify(Method(context.vehicularSignalHeadMock, setState)).Exactly(2);
        Verify(Method(context.vehicularSignalHeadMock, setState),
               Method(context.vehicularSignalHeadMock, setState)
                   .Using(State::AMBER_CONTINUOUS));
        Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
      }

      SECTION("it eventually changes state to RED_CONTINUOUS") {
        Timings timings = {.minimumRecallMs = 3000,
                           .amberClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        VehicularCyclePhase phase(context.config);

        phase.start();
        phase.update(1000);
        phase.update(1999);
        phase.update(1); // 3000 ms passed
        phase.update(1999);
        phase.update(1); // 5000 ms passed

        Verify(Method(context.vehicularSignalHeadMock, setState)).Exactly(3);
        Verify(Method(context.vehicularSignalHeadMock, setState),
               Method(context.vehicularSignalHeadMock, setState),
               Method(context.vehicularSignalHeadMock, setState)
                   .Using(State::RED_CONTINUOUS));

        Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
      }

      SECTION("it eventually calls onFinished") {
        Timings timings = {.minimumRecallMs = 3000,
                           .amberClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        VehicularCyclePhase phase(context.config);

        phase.start();
        phase.update(1000);
        phase.update(1999);
        phase.update(1); // 3000 ms passed
        phase.update(1999);
        phase.update(1); // 5000 ms passed
        phase.update(999);
        phase.update(1); // 6000 ms passed

        Verify(Method(context.vehicularSignalHeadMock, setState)).Exactly(3);
        Verify(Method(*context.onFinishedMock, operator())).Exactly(1);
      }
    }
  }
}
} // namespace
