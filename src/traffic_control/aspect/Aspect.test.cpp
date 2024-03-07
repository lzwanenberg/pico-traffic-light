#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "Aspect.hpp"

namespace {
using Aspect = TrafficControl::Aspect;
using Config = Aspect::Config;
using State = Aspect::State;

class DeviceControlMock {
public:
  std::vector<bool> calls;
  void controlDevice(bool state) { calls.push_back(state); };
  void reset() { calls.clear(); }
};

Config createConfig(DeviceControlMock &mock, int flashingIntervalMs = 500) {
  return {
      .flashingIntervalMs = flashingIntervalMs,
      .deviceControlFunction =
          [&mock](bool state) { mock.controlDevice(state); },
  };
}

TEST_CASE("Aspect") {
  SECTION(".setState") {
    SECTION("State::ON") {
      SECTION("it triggers device control function with value true") {
        DeviceControlMock mock;
        Aspect aspect(createConfig(mock));

        aspect.setState(State::ON);

        REQUIRE(mock.calls.size() == 1);
        REQUIRE(mock.calls[0] == true);
      }

      SECTION(".update") {
        SECTION("does not trigger additional device control function calls") {
          DeviceControlMock mock;
          Aspect aspect(createConfig(mock, 500));
          aspect.setState(State::ON);
          mock.reset();

          aspect.update(1200);

          REQUIRE(mock.calls.size() == 0);
        }
      }
    }

    SECTION("State::OFF") {
      SECTION("it triggers device control function with value false") {
        DeviceControlMock mock;
        Aspect aspect(createConfig(mock));

        aspect.setState(State::OFF);

        REQUIRE(mock.calls.size() == 1);
        REQUIRE(mock.calls[0] == false);
      }

      SECTION(".update") {
        SECTION("does not trigger additional device control function calls") {
          DeviceControlMock mock;
          Aspect aspect(createConfig(mock, 500));
          aspect.setState(State::OFF);
          mock.reset();

          aspect.update(1200);

          REQUIRE(mock.calls.size() == 0);
        }
      }
    }

    SECTION("State::FLASHING") {
      SECTION("it triggers device control function with value true") {
        DeviceControlMock mock;
        Aspect aspect(createConfig(mock));

        aspect.setState(State::FLASHING);

        REQUIRE(mock.calls.size() == 1);
        REQUIRE(mock.calls[0] == true);
      }

      SECTION(".update") {
        SECTION("triggers additional device control function calls with "
                "alternating values") {
          DeviceControlMock mock;
          Aspect aspect(createConfig(mock, 500));
          aspect.setState(State::FLASHING);
          mock.reset();

          aspect.update(400);
          aspect.update(400);
          aspect.update(400);
          aspect.update(400);
          aspect.update(400);
          aspect.update(400);
          aspect.update(400);

          REQUIRE(mock.calls.size() == 3);
          REQUIRE(mock.calls[0] == false);
          REQUIRE(mock.calls[1] == true);
          REQUIRE(mock.calls[2] == false);
        }
      }
    }
  }
}
} // namespace
