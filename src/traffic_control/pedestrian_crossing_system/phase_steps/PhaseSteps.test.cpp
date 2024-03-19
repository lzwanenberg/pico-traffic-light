#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

#include "PhaseSteps.hpp"

namespace {
using PhaseSteps = TrafficControl::PhaseSteps;
using PhaseStep = PhaseSteps::PhaseStep;
using Config = PhaseSteps::Config;
using FinishedCallback = PhaseSteps::FinishedCallback;

class FunctionMock {
public:
  int calls = 0;
  void call() { calls++; };
};

struct PhaseStepMock {
  FunctionMock executionFunction;
  PhaseStep step;
};

PhaseStep createStep(FunctionMock &executionFunction,
                     int initialDurationMs = 1000) {
  return {.initialDurationMs = initialDurationMs,
          .executionFunction = [&executionFunction]() {
            executionFunction.call();
          }};
}

TEST_CASE("PhaseSteps") {
  SECTION(".start") {
    SECTION("when no phases are given") {
      SECTION("it calls onFinished callback") {
        FunctionMock onFinished;

        PhaseSteps steps({.steps = std::vector<PhaseStep>{}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
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

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
        steps.registerFinishedListener(&callback);
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

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
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

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
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

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
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

  SECTION(".stop") {
    SECTION("it stops executing steps") {
      FunctionMock onFinished;

      FunctionMock step1Fn;
      FunctionMock step2Fn;

      PhaseStep step1 = createStep(step1Fn, 500);
      PhaseStep step2 = createStep(step2Fn, 1000);

      PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
      FinishedCallback callback = [&onFinished]() { onFinished.call(); };

      steps.registerFinishedListener(&callback);
      steps.start();
      steps.update(200);
      steps.stop();
      steps.update(500);
      steps.update(500);

      REQUIRE(step1Fn.calls == 1);
      REQUIRE(step2Fn.calls == 0);
      REQUIRE(onFinished.calls == 0);
    }
  }

  SECTION(".getCurrentStepIndex") {
    SECTION("when not running") {
      SECTION("it returns -1") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);

        REQUIRE(steps.getCurrentStepIndex() == -1);
      }
    }

    SECTION("when started but no time has passed") {
      SECTION("it returns 0") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
        steps.start();

        REQUIRE(steps.getCurrentStepIndex() == 0);
      }
    }

    SECTION("when second step is being executed") {
      SECTION("it returns 1") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
        steps.start();
        steps.update(200);
        steps.update(350);

        REQUIRE(steps.getCurrentStepIndex() == 1);
      }
    }

    SECTION("when all steps have been executed") {
      SECTION("it returns -1") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
        steps.start();
        steps.update(200);
        steps.update(299);
        steps.update(100);
        steps.update(500);
        steps.update(500);

        REQUIRE(steps.getCurrentStepIndex() == -1);
      }
    }

    SECTION("when .stop has been called") {
      SECTION("it returns -1") {
        FunctionMock onFinished;

        FunctionMock step1Fn;
        FunctionMock step2Fn;

        PhaseStep step1 = createStep(step1Fn, 500);
        PhaseStep step2 = createStep(step2Fn, 1000);

        PhaseSteps steps({.steps = std::vector<PhaseStep>{step1, step2}});
        FinishedCallback callback = [&onFinished]() { onFinished.call(); };

        steps.registerFinishedListener(&callback);
        steps.start();
        steps.update(200);
        steps.stop();
        steps.update(500);
        steps.update(500);

        REQUIRE(steps.getCurrentStepIndex() == -1);
      }
    }
  }
}
} // namespace
