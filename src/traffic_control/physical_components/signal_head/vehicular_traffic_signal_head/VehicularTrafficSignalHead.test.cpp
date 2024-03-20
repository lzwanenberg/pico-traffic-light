#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "VehicularTrafficSignalHead.hpp"

namespace {
using Aspect = TrafficControl::Aspect;
using VehicularTrafficSignalHead = TrafficControl::VehicularTrafficSignalHead;
using Config = VehicularTrafficSignalHead::Config;
using State = VehicularTrafficSignalHead::State;

class DeviceControlMock {
public:
  std::vector<bool> calls;
  void controlDevice(bool state) { calls.push_back(state); };
  void reset() { calls.clear(); }
};

class DeviceControlMocks {
public:
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
  DeviceControlMock red;
  DeviceControlMock amber;
  DeviceControlMock green;

  void reset() {
    red.reset();
    amber.reset();
    green.reset();
  }
};

static Config createConfig(DeviceControlMocks &mocks,
                           int flashingIntervalMs = 500) {
  return {.flashingIntervalMs = flashingIntervalMs,
          .deviceControls = {
              .red = [&aspect = mocks.red](
                         bool state) { aspect.controlDevice(state); },
              .amber = [&aspect = mocks.amber](
                           bool state) { aspect.controlDevice(state); },
              .green = [&aspect = mocks.green](
                           bool state) { aspect.controlDevice(state); }}};
}

TEST_CASE("VehicularTrafficSignalHead") {
  SECTION(".setState") {
    SECTION("State::RED_CONTINUOUS") {
      SECTION("it sets red aspect to State::ON") {
        DeviceControlMocks mocks;
        VehicularTrafficSignalHead signalHead(createConfig(mocks));
        signalHead.setState(
            TrafficControl::VehicularTrafficSignalHead::State::RED_CONTINUOUS);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.amber.calls.size() == 1);
        REQUIRE(mocks.green.calls.size() == 1);

        REQUIRE(mocks.red.calls[0] == true);
        REQUIRE(mocks.amber.calls[0] == false);
        REQUIRE(mocks.green.calls[0] == false);
      }

      SECTION(".update") {
        SECTION("does not perform any more subsequent calls") {
          DeviceControlMocks mocks;
          VehicularTrafficSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::RED_CONTINUOUS);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.red.calls.size() == 0);
          REQUIRE(mocks.amber.calls.size() == 0);
          REQUIRE(mocks.green.calls.size() == 0);
        }
      }
    }

    SECTION("State::AMBER_CONTINUOUS") {
      SECTION("it sets amber aspect to State::ON") {
        DeviceControlMocks mocks;
        VehicularTrafficSignalHead signalHead(createConfig(mocks));
        signalHead.setState(TrafficControl::VehicularTrafficSignalHead::State::
                                AMBER_CONTINUOUS);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.amber.calls.size() == 1);
        REQUIRE(mocks.green.calls.size() == 1);

        REQUIRE(mocks.red.calls[0] == false);
        REQUIRE(mocks.amber.calls[0] == true);
        REQUIRE(mocks.green.calls[0] == false);
      }

      SECTION(".update") {
        SECTION("does not perform any more subsequent calls") {
          DeviceControlMocks mocks;
          VehicularTrafficSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::AMBER_CONTINUOUS);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.red.calls.size() == 0);
          REQUIRE(mocks.amber.calls.size() == 0);
          REQUIRE(mocks.green.calls.size() == 0);
        }
      }
    }

    SECTION("State::GREEN_CONTINUOUS") {
      SECTION("it sets green aspect to State::ON") {
        DeviceControlMocks mocks;
        VehicularTrafficSignalHead signalHead(createConfig(mocks));
        signalHead.setState(TrafficControl::VehicularTrafficSignalHead::State::
                                GREEN_CONTINUOUS);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.amber.calls.size() == 1);
        REQUIRE(mocks.green.calls.size() == 1);

        REQUIRE(mocks.red.calls[0] == false);
        REQUIRE(mocks.amber.calls[0] == false);
        REQUIRE(mocks.green.calls[0] == true);
      }

      SECTION(".update") {
        SECTION("does not perform any more subsequent calls") {
          DeviceControlMocks mocks;
          VehicularTrafficSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::GREEN_CONTINUOUS);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.red.calls.size() == 0);
          REQUIRE(mocks.amber.calls.size() == 0);
          REQUIRE(mocks.green.calls.size() == 0);
        }
      }
    }

    SECTION("State::RED_FLASHING") {
      SECTION("it sets red aspect to State::ON") {
        DeviceControlMocks mocks;
        VehicularTrafficSignalHead signalHead(createConfig(mocks));

        signalHead.setState(State::RED_FLASHING);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.red.calls[0] == true);
        REQUIRE(mocks.amber.calls.size() == 1);
        REQUIRE(mocks.amber.calls[0] == false);
        REQUIRE(mocks.green.calls.size() == 1);
        REQUIRE(mocks.green.calls[0] == false);
      }

      SECTION(".update") {
        SECTION("flashes red aspect") {
          DeviceControlMocks mocks;
          VehicularTrafficSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::RED_FLASHING);
          mocks.reset();

          signalHead.update(100);
          REQUIRE(mocks.red.calls.size() == 0);

          signalHead.update(300);
          signalHead.update(300);
          REQUIRE(mocks.red.calls.size() == 1);
          REQUIRE(mocks.red.calls[0] == false);
          mocks.reset();

          signalHead.update(500);
          REQUIRE(mocks.red.calls.size() == 1);
          REQUIRE(mocks.red.calls[0] == true);
        }

        SECTION("does not perform any more subsequent calls on other aspects") {
          DeviceControlMocks mocks;
          VehicularTrafficSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::RED_FLASHING);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.amber.calls.size() == 0);
          REQUIRE(mocks.green.calls.size() == 0);
        }
      }
    }

    SECTION("State::AMBER_FLASHING") {
      SECTION("it sets amber aspect to State::ON") {
        DeviceControlMocks mocks;
        VehicularTrafficSignalHead signalHead(createConfig(mocks));

        signalHead.setState(State::AMBER_FLASHING);

        REQUIRE(mocks.red.calls.size() == 1);
        REQUIRE(mocks.red.calls[0] == false);
        REQUIRE(mocks.amber.calls.size() == 1);
        REQUIRE(mocks.amber.calls[0] == true);
        REQUIRE(mocks.green.calls.size() == 1);
        REQUIRE(mocks.green.calls[0] == false);
      }

      SECTION(".update") {
        SECTION("flashes amber aspect") {
          DeviceControlMocks mocks;
          VehicularTrafficSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::AMBER_FLASHING);
          mocks.reset();

          signalHead.update(100);
          REQUIRE(mocks.amber.calls.size() == 0);

          signalHead.update(300);
          signalHead.update(300);
          REQUIRE(mocks.amber.calls.size() == 1);
          REQUIRE(mocks.amber.calls[0] == false);
          mocks.reset();

          signalHead.update(500);
          REQUIRE(mocks.amber.calls.size() == 1);
          REQUIRE(mocks.amber.calls[0] == true);
        }

        SECTION("does not perform any more subsequent calls on other aspects") {
          DeviceControlMocks mocks;
          VehicularTrafficSignalHead signalHead(createConfig(mocks, 500));
          signalHead.setState(State::AMBER_FLASHING);
          mocks.reset();

          signalHead.update(1200);

          REQUIRE(mocks.red.calls.size() == 0);
          REQUIRE(mocks.green.calls.size() == 0);
        }
      }
    }
  }
}
} // namespace
