#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

#include "PhaseStep.hpp"

namespace {
using PhaseStep = TrafficControl::PhaseStep;
using Config = PhaseStep::Config;
using FinishedCallback = PhaseStep::FinishedCallback;

class FunctionMock {
public:
  int calls = 0;
  void call() { calls++; };
};

TEST_CASE("PhaseStep") {
  SECTION(".start") {
    SECTION("it calls executionFunction of first phase") {
      FunctionMock execute;
      FunctionMock onFinished;

      PhaseStep step({{.initialDurationMs = 1000,
                       .executionFunction = [&execute]() { execute.call(); }}});

      FinishedCallback callback = [&onFinished]() { onFinished.call(); };

      step.registerFinishedListener(&callback);
      step.start();

      REQUIRE(execute.calls == 1);
      REQUIRE(onFinished.calls == 0);
    }
  }

  SECTION(".update") {
    SECTION("when not enough time has passed") {
      SECTION("it does not trigger onFinished callback") {
        int initialDurationMs = 1000;

        FunctionMock execute;
        FunctionMock onFinished;
        PhaseStep step(
            {{.initialDurationMs = initialDurationMs,
              .executionFunction = [&execute]() { execute.call(); }}});

        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        step.registerFinishedListener(&callback);
        step.start();
        step.update(400);
        step.update(400);

        REQUIRE(onFinished.calls == 0);
      }
    }

    SECTION("when enough time has passed") {
      SECTION("it triggers onFinished callback") {
        int initialDurationMs = 1000;

        FunctionMock execute;
        FunctionMock onFinished;
        PhaseStep step(
            {{.initialDurationMs = initialDurationMs,
              .executionFunction = [&execute]() { execute.call(); }}});

        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        step.registerFinishedListener(&callback);
        step.start();
        step.update(400);
        step.update(400);
        step.update(400);

        REQUIRE(onFinished.calls == 1);
      }
    }
  }

  SECTION(".extend") {
    SECTION("when remaining time is less than extension time") {
      SECTION("it extends the step with provided time") {
        int initialDurationMs = 1000;

        FunctionMock execute;
        FunctionMock onFinished;
        PhaseStep step(
            {{.initialDurationMs = initialDurationMs,
              .executionFunction = [&execute]() { execute.call(); }}});

        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        step.registerFinishedListener(&callback);
        step.start();
        step.update(400);
        step.update(400);
        step.extend(500);
        step.update(400);
        step.update(50);

        REQUIRE(onFinished.calls == 0);

        step.update(200);

        REQUIRE(onFinished.calls == 1);
      }
    }

    SECTION("when remaining time is more than extension time") {
      SECTION("it does not affect execution time") {
        int initialDurationMs = 1000;

        FunctionMock execute;
        FunctionMock onFinished;
        PhaseStep step(
            {{.initialDurationMs = initialDurationMs,
              .executionFunction = [&execute]() { execute.call(); }}});

        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        step.registerFinishedListener(&callback);
        step.start();
        step.extend(900);
        step.update(400);
        step.update(400);
        step.extend(150);
        step.update(400);

        REQUIRE(onFinished.calls == 1);
      }
    }
  }
}
} // namespace
