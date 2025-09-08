#include <swgtk/Math.hpp>
#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN(readability-magic-numbers, *-avoid-magic-numbers)
TEST_CASE("Math Tests") {
  SECTION("Test expected cases") {
    REQUIRE(swgtk::math::DegreesToRadians(90.0f) == std::numbers::pi_v<float> * 0.5f);
    REQUIRE(swgtk::math::DegreesToRadians(180.0) == std::numbers::pi);
    REQUIRE(swgtk::math::RadiansToDegrees(std::numbers::pi_v<float> * 0.25f) == 45.0f);
    REQUIRE(swgtk::math::RadiansToDegrees(std::numbers::pi * 2.0) == 360.0);
  }
}
// NOLINTEND(readability-magic-numbers, *-avoid-magic-numbers)