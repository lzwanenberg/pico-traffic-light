#define CATCH_CONFIG_MAIN
#include "PedestrianCrossingSystem.hpp"
#include "../push_button/IPushButton.hpp"
#include "cycle_phases/pedestrian_cycle_phase/IPedestrianCyclePhase.hpp"
#include "cycle_phases/vehicular_cycle_phase/IVehicularCyclePhase.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fakeit.hpp>
#include <iostream>

namespace {
using namespace fakeit;
using PedestrianCrossingSystem = TrafficControl::PedestrianCrossingSystem;
using IPedestrianCyclePhase = TrafficControl::IPedestrianCyclePhase;
using IVehicularCyclePhase = TrafficControl::IVehicularCyclePhase;
using IPushButton = TrafficControl::IPushButton;
using Config = TrafficControl::PedestrianCrossingSystem::Config;

struct PedestrianCyclePhaseMock {
  Mock<IPedestrianCyclePhase> mock;
  IPedestrianCyclePhase::FinishedCallback *finish;
};

struct VehicularCyclePhaseMock {
  Mock<IVehicularCyclePhase> mock;
  IVehicularCyclePhase::FinishedCallback *finish;
};

struct TestContext {
  Config config;

  PedestrianCyclePhaseMock pedestrianCyclePhase;
  VehicularCyclePhaseMock vehicularCyclePhase;
};

void initializePedestrianCyclePhaseMock(TestContext &context) {
  context.config.pedestrianPhase = &context.pedestrianCyclePhase.mock.get();

  When(Method(context.pedestrianCyclePhase.mock, registerFinishedListener))
      .Do([&](IPedestrianCyclePhase::FinishedCallback *callback) {
        context.pedestrianCyclePhase.finish = callback;
      });
  When(Method(context.pedestrianCyclePhase.mock, start)).AlwaysReturn();
  When(Method(context.pedestrianCyclePhase.mock, reset)).AlwaysReturn();
  When(Method(context.pedestrianCyclePhase.mock, update)).AlwaysReturn();
  When(Method(context.pedestrianCyclePhase.mock, isRequested))
      .AlwaysReturn(true); // TODO
}

void initializeVehicularCyclePhaseMock(TestContext &context) {
  context.config.vehicularPhase = &context.vehicularCyclePhase.mock.get();

  When(Method(context.vehicularCyclePhase.mock, registerFinishedListener))
      .Do([&](IVehicularCyclePhase::FinishedCallback *callback) {
        context.vehicularCyclePhase.finish = callback;
      });
  When(Method(context.vehicularCyclePhase.mock, start)).AlwaysReturn();
  When(Method(context.vehicularCyclePhase.mock, reset)).AlwaysReturn();
  When(Method(context.vehicularCyclePhase.mock, update)).AlwaysReturn();
}

void initializeTestContext(TestContext &context) {
  initializePedestrianCyclePhaseMock(context);
  initializeVehicularCyclePhaseMock(context);
}

} // namespace

TEST_CASE("PedestrianCrossingSystem") {
  SECTION(".start") {

    SECTION("resets phases") {
      TestContext context;
      initializeTestContext(context);
      PedestrianCrossingSystem system(context.config);

      system.start();

      Verify(Method(context.vehicularCyclePhase.mock, reset)).Exactly(1);
      Verify(Method(context.pedestrianCyclePhase.mock, reset)).Exactly(1);
    }

    SECTION("starts vehicular phase") {
      TestContext context;
      initializeTestContext(context);
      PedestrianCrossingSystem system(context.config);

      system.start();

      Verify(Method(context.vehicularCyclePhase.mock, start)).Exactly(1);
      Verify(Method(context.pedestrianCyclePhase.mock, start)).Exactly(0);
    }
  }

  SECTION(".update") {
    SECTION("updates all phases") {
      TestContext context;
      initializeTestContext(context);
      PedestrianCrossingSystem system(context.config);

      system.start();
      system.update(300);
      system.update(500);

      Verify(Method(context.vehicularCyclePhase.mock, update).Using(300),
             Method(context.vehicularCyclePhase.mock, update).Using(500));

      Verify(Method(context.pedestrianCyclePhase.mock, update).Using(300),
             Method(context.pedestrianCyclePhase.mock, update).Using(500));
    }
  }

  SECTION("when vehicular cycle phase finished") {
    SECTION("it starts pedestrian cycle phase") {
      TestContext context;
      initializeTestContext(context);
      PedestrianCrossingSystem system(context.config);

      system.start();
      (*context.vehicularCyclePhase.finish)();

      Verify(Method(context.vehicularCyclePhase.mock, start)).Exactly(1);
      Verify(Method(context.pedestrianCyclePhase.mock, start)).Exactly(1);
    }
  }

  SECTION("when vehicular cycle phase finished") {
    SECTION("it starts pedestrian cycle phase") {
      TestContext context;
      initializeTestContext(context);
      PedestrianCrossingSystem system(context.config);

      system.start();
      (*context.vehicularCyclePhase.finish)();
      (*context.pedestrianCyclePhase.finish)();

      Verify(Method(context.vehicularCyclePhase.mock, start)).Exactly(2);
      Verify(Method(context.pedestrianCyclePhase.mock, start)).Exactly(1);
    }
  }
}
