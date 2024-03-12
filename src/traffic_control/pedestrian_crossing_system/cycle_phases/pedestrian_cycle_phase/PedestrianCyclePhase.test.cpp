#define CATCH_CONFIG_MAIN
#include "PedestrianCyclePhase.hpp"
#include "../../../signal_head/pedestrian_signal_head/IPedestrianSignalHead.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fakeit.hpp>
#include <iostream>
#include <vector>

namespace {
using namespace fakeit;
using PedestrianCyclePhase = TrafficControl::PedestrianCyclePhase;
using Config = PedestrianCyclePhase::Config;
using FinishedCallback = PedestrianCyclePhase::FinishedCallback;
using IPedestrianSignalHead = TrafficControl::IPedestrianSignalHead;
using State = IPedestrianSignalHead::State;
using Timings = Config::Timings;

class FinishedCallbackWrapper {
public:
  FinishedCallbackWrapper() = default;
  virtual ~FinishedCallbackWrapper() = default;
  virtual void operator()() = 0;
};

struct TestContext {
  Config config;
  FinishedCallback finishedCallback;
  std::shared_ptr<Mock<FinishedCallbackWrapper>> onFinishedMock;
  Mock<IPedestrianSignalHead> pedestrianSignalHeadMock;
};

void initializeTestContext(TestContext &context,
                           Timings timings = {
                               .minimumRecallMs = 3000,
                               .greenFlashingClearanceTimeMs = 2000,
                               .redClearanceTimeMs = 1000}) {
  context.onFinishedMock = std::make_shared<Mock<FinishedCallbackWrapper>>();
  IPedestrianSignalHead &pedestrianSignalHead =
      context.pedestrianSignalHeadMock.get();

  context.finishedCallback = [&context] {
    (*context.onFinishedMock).get().operator()();
  };
  context.config = {.pedestrianSignalHead = &pedestrianSignalHead,
                    .timings = timings};

  When(Method(*context.onFinishedMock, operator())).AlwaysReturn();
  When(Method(context.pedestrianSignalHeadMock, setState)).AlwaysReturn();
  When(Method(context.pedestrianSignalHeadMock, update)).AlwaysReturn();
}

TEST_CASE("PedestrianCyclePhase") {
  SECTION(".start") {
    SECTION("it set states GREEN_CONTINUOUS") {
      TestContext context;
      initializeTestContext(context);
      PedestrianCyclePhase phase(context.config);

      phase.registerFinishedListener(&context.finishedCallback);
      phase.start();

      Verify(Method(context.pedestrianSignalHeadMock, setState)
                 .Using(State::GREEN_CONTINUOUS))
          .Exactly(1);
      Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
    }
  }

  SECTION(".update") {
    SECTION("when not enough time has passed") {
      SECTION("it does not change state") {
        Timings timings = {.minimumRecallMs = 3000,
                           .greenFlashingClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        PedestrianCyclePhase phase(context.config);

        phase.registerFinishedListener(&context.finishedCallback);
        phase.start();
        phase.update(1000);
        phase.update(1999);

        Verify(Method(context.pedestrianSignalHeadMock, setState)).Exactly(1);
        Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
      }
    }

    SECTION("when enough time has passed") {
      SECTION("it eventually changes state to AMBER_CONTINUOUS") {
        Timings timings = {.minimumRecallMs = 3000,
                           .greenFlashingClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        PedestrianCyclePhase phase(context.config);

        phase.registerFinishedListener(&context.finishedCallback);
        phase.start();
        phase.update(1000);
        phase.update(1999);
        phase.update(1); // 3000 ms passed (minimumRecallMs)

        Verify(Method(context.pedestrianSignalHeadMock, setState)).Exactly(2);
        Verify(Method(context.pedestrianSignalHeadMock, setState),
               Method(context.pedestrianSignalHeadMock, setState)
                   .Using(State::GREEN_FLASHING));
        Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
      }

      SECTION("it eventually changes state to RED_CONTINUOUS") {
        Timings timings = {.minimumRecallMs = 3000,
                           .greenFlashingClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        PedestrianCyclePhase phase(context.config);

        phase.registerFinishedListener(&context.finishedCallback);
        phase.start();
        phase.update(1000);
        phase.update(1999);
        phase.update(1); // 3000 ms passed
        phase.update(1999);
        phase.update(1); // 5000 ms passed

        Verify(Method(context.pedestrianSignalHeadMock, setState)).Exactly(3);
        Verify(Method(context.pedestrianSignalHeadMock, setState),
               Method(context.pedestrianSignalHeadMock, setState),
               Method(context.pedestrianSignalHeadMock, setState)
                   .Using(State::RED_CONTINUOUS));

        Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
      }

      SECTION("it eventually calls onFinished") {
        Timings timings = {.minimumRecallMs = 3000,
                           .greenFlashingClearanceTimeMs = 2000,
                           .redClearanceTimeMs = 1000};
        TestContext context;
        initializeTestContext(context, timings);
        PedestrianCyclePhase phase(context.config);

        phase.registerFinishedListener(&context.finishedCallback);
        phase.start();
        phase.update(1000);
        phase.update(1999);
        phase.update(1); // 3000 ms passed
        phase.update(1999);
        phase.update(1); // 5000 ms passed
        phase.update(999);
        phase.update(1); // 6000 ms passed

        Verify(Method(context.pedestrianSignalHeadMock, setState)).Exactly(3);
        Verify(Method(*context.onFinishedMock, operator())).Exactly(1);
      }
    }

    SECTION("it updates signal head") {
      TestContext context;
      initializeTestContext(context);
      PedestrianCyclePhase phase(context.config);

      phase.registerFinishedListener(&context.finishedCallback);
      phase.start();
      phase.update(100);
      phase.update(300);
      phase.update(200);

      Verify(Method(context.pedestrianSignalHeadMock, update)).Exactly(3);
      Verify(Method(context.pedestrianSignalHeadMock, update).Using(100),
             Method(context.pedestrianSignalHeadMock, update).Using(300),
             Method(context.pedestrianSignalHeadMock, update).Using(200));
    }
  }

  SECTION(".reset") {
    SECTION("it stops executing steps and sets state to RED_CONTINOUS") {
      Timings timings = {.minimumRecallMs = 3000,
                         .greenFlashingClearanceTimeMs = 2000,
                         .redClearanceTimeMs = 1000};
      TestContext context;
      initializeTestContext(context, timings);
      PedestrianCyclePhase phase(context.config);

      phase.registerFinishedListener(&context.finishedCallback);
      phase.start();
      phase.update(1000);
      phase.update(1999);
      phase.reset();
      phase.update(1);
      phase.update(1999);
      phase.update(1);

      Verify(Method(context.pedestrianSignalHeadMock, setState)).Exactly(2);
      Verify(Method(context.pedestrianSignalHeadMock, setState)
                 .Using(State::GREEN_CONTINUOUS),
             Method(context.pedestrianSignalHeadMock, setState)
                 .Using(State::RED_CONTINUOUS));

      Verify(Method(*context.onFinishedMock, operator())).Exactly(0);
    }
  }
}
} // namespace
