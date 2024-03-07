#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../../aspect/Aspect.hpp"
#include "PedestrianSignalHead.hpp"

using Aspect = TrafficControl::Aspect;
using PedestrianSignalHead = TrafficControl::PedestrianSignalHead;
using Config = PedestrianSignalHead::Config;
using State = PedestrianSignalHead::State;

class DeviceControlMock {
public:
  std::vector<bool> calls;
  void controlDevice(bool state) { calls.push_back(state); };
  void reset() { calls.clear(); }
};

class DeviceControlMocks {
public:
  DeviceControlMock red;
  DeviceControlMock green;
  void reset() {
    red.reset();
    green.reset();
  }
};

Config createConfig(DeviceControlMocks &mocks, int flashingIntervalMs = 500) {
  return {.flashingIntervalMs = flashingIntervalMs,
          .deviceControls = {
              .red = [&aspect = mocks.red](
                         bool state) { aspect.controlDevice(state); },
              .green = [&aspect = mocks.green](
                           bool state) { aspect.controlDevice(state); }}};
}

TEST_CASE("PedestrianSignalHead") {
  SECTION(".setState") {
    SECTION("State::RED_CONTINUOUS") {
      SECTION("it sets red aspect to State::ON") {
        DeviceControlMocks mocks;
        PedestrianSignalHead signalHead(createConfig(mocks));

        signalHead.setState(State::RED_CONTINUOUS);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.red.calls[0] == true);
        REQUIRE(mocks.green.calls.size() == 1);
        REQUIRE(mocks.green.calls[0] == false);
      }

      SECTION(".update") {
        SECTION("does not perform any more subsequent calls") {
          DeviceControlMocks mocks;
          PedestrianSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::RED_CONTINUOUS);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.red.calls.size() == 0);
          REQUIRE(mocks.green.calls.size() == 0);
        }
      }
    }

    SECTION("State::GREEN_CONTINUOUS") {
      SECTION("it sets red aspect to State::ON") {
        DeviceControlMocks mocks;
        PedestrianSignalHead signalHead(createConfig(mocks));

        signalHead.setState(State::GREEN_CONTINUOUS);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.red.calls[0] == false);
        REQUIRE(mocks.green.calls.size() == 1);
        REQUIRE(mocks.green.calls[0] == true);
      }

      SECTION(".update") {
        SECTION("does not perform any more subsequent calls") {
          DeviceControlMocks mocks;
          PedestrianSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::GREEN_CONTINUOUS);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.red.calls.size() == 0);
          REQUIRE(mocks.green.calls.size() == 0);
        }
      }
    }

    SECTION("State::FLASHING_GREEN") {
      SECTION("it sets green aspect to State::ON") {
        DeviceControlMocks mocks;
        PedestrianSignalHead signalHead(createConfig(mocks));

        signalHead.setState(State::FLASHING_GREEN);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.red.calls[0] == false);
        REQUIRE(mocks.green.calls.size() == 1);
        REQUIRE(mocks.green.calls[0] == true);
      }

      SECTION(".update") {
        SECTION("flashes green aspect") {
          DeviceControlMocks mocks;
          PedestrianSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::FLASHING_GREEN);
          mocks.reset();

          signalHead.update(100);
          REQUIRE(mocks.green.calls.size() == 0);

          signalHead.update(300);
          signalHead.update(300);
          REQUIRE(mocks.green.calls.size() == 1);
          REQUIRE(mocks.green.calls[0] == false);
          mocks.reset();

          signalHead.update(500);
          REQUIRE(mocks.green.calls.size() == 1);
          REQUIRE(mocks.green.calls[0] == true);
        }

        SECTION("does not perform any more subsequent calls on red aspect") {
          DeviceControlMocks mocks;
          PedestrianSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::FLASHING_GREEN);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.red.calls.size() == 0);
        }
      }
    }
  }
}
