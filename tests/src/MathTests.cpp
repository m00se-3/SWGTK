#include <swgtk/Math.hpp>
#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN(readability-magic-numbers, *-avoid-magic-numbers)
TEST_CASE("Angle Tests") {
  SECTION("Test type traits") {
    STATIC_REQUIRE(std::is_trivially_constructible_v<swgtk::Degrees<double>>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<swgtk::Degrees<float>>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<swgtk::Radians<float>>);
    STATIC_REQUIRE(std::is_trivially_assignable_v<swgtk::Radians<double>, swgtk::Radians<double>>);
  }

  SECTION("Test expected cases") {
    STATIC_REQUIRE(swgtk::DegreesToRadians(swgtk::Degrees{ 90.0f }) == swgtk::Radians{ std::numbers::pi_v<float> * 0.5f });
    STATIC_REQUIRE(swgtk::DegreesToRadians(swgtk::Degrees{ 180.0 }) == swgtk::Radians{ std::numbers::pi });
    STATIC_REQUIRE(swgtk::RadiansToDegrees(swgtk::Radians{ std::numbers::pi_v<float> * 0.25f }) == swgtk::Degrees{ 45.0f });
    STATIC_REQUIRE(swgtk::RadiansToDegrees(swgtk::Radians{ std::numbers::pi * 2.0 }) == swgtk::Degrees{ 360.0 });
  }

  SECTION("Test out-of-bounds input") {
    STATIC_REQUIRE_FALSE(swgtk::Degrees{560.0}.value() == 560.0);
    STATIC_REQUIRE_FALSE(swgtk::Radians{-560.0}.value() == -560.0);
    STATIC_REQUIRE(swgtk::Radians{ std::numbers::pi * 20.0 }.value() == std::numbers::pi * 2.0);
    STATIC_REQUIRE(swgtk::Degrees{ -40000.0f }.value() == -360.0f);
  }
}
// NOLINTEND(readability-magic-numbers, *-avoid-magic-numbers)