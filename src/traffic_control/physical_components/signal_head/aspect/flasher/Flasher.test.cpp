#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

#include "Flasher.hpp"

namespace {
using Flasher = TrafficControl::Flasher;
using Config = Flasher::Config;

class StateChangeMock {
public:
  std::vector<Flasher::State> calls;
  void onStateChange(Flasher::State state) { calls.push_back(state); };
  void reset() { calls.clear(); }
};

Config createConfig(StateChangeMock &mock, int intervalMs = 500) {
  return {.intervalMs = intervalMs,
          .onStateChange = [&mock](Flasher::State state) {
            mock.onStateChange(state);
          }};
}

TEST_CASE("Flasher") {

  SECTION(".start") {
    SECTION("it triggers callback with value State::ON") {
      StateChangeMock mock;
      Flasher flasher(createConfig(mock));

      flasher.start();

      REQUIRE(mock.calls.size() == 1);
      REQUIRE(mock.calls[0] == Flasher::State::ON);
    }
  }

  SECTION(".update") {
    SECTION("when elapsed time does not exceed interval") {
      SECTION("it does not trigger callback") {
        StateChangeMock mock;
        int intervalMs = 500;
        Flasher flasher(createConfig(mock, intervalMs));
        flasher.start();
        mock.reset();

        flasher.update(100);
        flasher.update(300);
        flasher.update(99);

        REQUIRE(mock.calls.size() == 0);
      }
    }

    SECTION("when elapsed time exceeds interval") {
      SECTION("it triggers callback with value State::OFF") {
        StateChangeMock mock;
        int intervalMs = 500;
        Flasher flasher(createConfig(mock, intervalMs));
        flasher.start();
        mock.reset();

        flasher.update(100);
        flasher.update(300);
        flasher.update(100);

        REQUIRE(mock.calls.size() == 1);
        REQUIRE(mock.calls[0] == Flasher::State::OFF);
      }
    }

    SECTION("when elapsed time exceeds interval twice") {
      SECTION("it triggers callback with value back to State::ON") {
        StateChangeMock mock;
        int intervalMs = 500;
        Flasher flasher(createConfig(mock, intervalMs));
        flasher.start();
        flasher.update(500);
        mock.reset();

        flasher.update(100);
        flasher.update(300);
        flasher.update(100);

        REQUIRE(mock.calls.size() == 1);
        REQUIRE(mock.calls[0] == Flasher::State::ON);
      }
    }
  }
}
} // namespace
