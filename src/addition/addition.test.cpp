#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "addition.hpp"

TEST_CASE("Additions are computed") {
  REQUIRE(Addition(1, 1) == 2);
  REQUIRE(Addition(3, 6) == 9);
  REQUIRE(Addition(12, 17) == 29);
}
