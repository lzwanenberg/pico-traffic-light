#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

#include "PhaseSteps.hpp"

namespace {
using PhaseSteps = TrafficControl::PhaseSteps;
using PhaseStep = PhaseSteps::PhaseStep;
using Config = PhaseSteps::Config;

class FunctionMock {
public:
  int calls = 0;
  void call() { calls++; };
};

struct PhaseStepMock {
  FunctionMock executionFunction;
  PhaseStep step;
};

PhaseStep createStep(FunctionMock &executionFunction, int durationMs = 1000) {
  return {.durationMs = durationMs,
          .executionFunction = [&executionFunction]() {
            executionFunction.call();
          }};
}

TEST_CASE("PhaseSteps") {
  SECTION(".start") {
    SECTION("when no phases are given") {
      SECTION("it calls onFinished callback") {
        FunctionMock onFinished;
        PhaseSteps steps(
            {.steps = std::vector<PhaseStep>{},
             .onFinished = [&onFinished]() { onFinished.call(); }});

        steps.start();

        REQUIRE(onFinished.calls == 1);
      }
    }

    SECTION("when phases are given") {
      SECTION("it calls executionFunction of first phase") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn);
        PhaseStep step2 = createStep(step2Fn);

        PhaseSteps steps(
            {.steps = std::vector<PhaseStep>{step1, step2},
             .onFinished = [&onFinished]() { onFinished.call(); }});

        steps.start();

        REQUIRE(step1Fn.calls == 1);
        REQUIRE(step2Fn.calls == 0);
        REQUIRE(onFinished.calls == 0);
      }
    }
  }

  SECTION(".update") {
    SECTION("when not enough time has passed for step 2") {
      SECTION("it does not call executionFunction for step 2") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps(
            {.steps = std::vector<PhaseStep>{step1, step2},
             .onFinished = [&onFinished]() { onFinished.call(); }});

        steps.start();
        steps.update(200);
        steps.update(299);

        REQUIRE(step1Fn.calls == 1);
        REQUIRE(step2Fn.calls == 0);
        REQUIRE(onFinished.calls == 0);
      }
    }

    SECTION("when enough time has passed for step 2") {
      SECTION("it calls executionFunction for step 2") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps(
            {.steps = std::vector<PhaseStep>{step1, step2},
             .onFinished = [&onFinished]() { onFinished.call(); }});

        steps.start();
        steps.update(200);
        steps.update(299);
        steps.update(1);

        REQUIRE(step1Fn.calls == 1);
        REQUIRE(step2Fn.calls == 1);
        REQUIRE(onFinished.calls == 0);
      }
    }

    SECTION("when enough time has passed for last step") {
      SECTION("it calls onFinished") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps(
            {.steps = std::vector<PhaseStep>{step1, step2},
             .onFinished = [&onFinished]() { onFinished.call(); }});

        steps.start();
        steps.update(200);
        steps.update(299);
        steps.update(1);
        steps.update(500);
        steps.update(500);

        REQUIRE(step1Fn.calls == 1);
        REQUIRE(step2Fn.calls == 1);
        REQUIRE(onFinished.calls == 1);
      }
    }
  }
}
} // namespace
