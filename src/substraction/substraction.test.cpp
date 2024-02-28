#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "substraction.hpp"

TEST_CASE("Substractions are computed", "[substraction]") {
  REQUIRE(Substraction(1, 1) == 0);
  REQUIRE(Substraction(6, 3) == 3);
  REQUIRE(Substraction(17, 12) == 5);
}
