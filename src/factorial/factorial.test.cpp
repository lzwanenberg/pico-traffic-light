#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "factorial.hpp"

TEST_CASE("Factorials are computed") {
  REQUIRE(Factorial(1) == 1);
  REQUIRE(Factorial(2) == 2);
  REQUIRE(Factorial(3) == 6);
  REQUIRE(Factorial(10) == 3628800);
}
