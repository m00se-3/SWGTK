#include <catch2/catch_test_macros.hpp>
#include <swgtk/Math.hpp>

// NOLINTBEGIN(readability-magic-numbers, *-avoid-magic-numbers)

// Attempting to convince msvc to test the helper functions at compile-time

static constexpr auto expected1 = swgtk::DegreesToRadians(swgtk::Degrees{90.0f}) == swgtk::Radians{std::numbers::pi_v<float> * 0.5f};
static constexpr auto expected2 = swgtk::DegreesToRadians(swgtk::Degrees{180.0}) == swgtk::Radians{std::numbers::pi};
static constexpr auto expected3 = swgtk::RadiansToDegrees(swgtk::Radians{std::numbers::pi_v<float> * 0.25f}) == swgtk::Degrees{45.0f};
static constexpr auto expected4 = swgtk::RadiansToDegrees(swgtk::Radians{std::numbers::pi * 2.0}) == swgtk::Degrees{360.0};
static constexpr auto outOfBounds = [] consteval { return swgtk::DegreesToRadians(swgtk::Degs{540.0} - 45.0) == swgtk::Rads{ std::numbers::pi * 1.75 }; }();
static constexpr auto outOfBounds1 = [] consteval { return swgtk::RadiansToDegrees(swgtk::Rads{ std::numbers::pi } + (std::numbers::pi * 30.0)) == swgtk::Degs{ 180.0 }; }();

TEST_CASE("Angle Tests") {
  SECTION("Test type traits") {
    STATIC_REQUIRE(std::is_trivially_destructible_v<swgtk::Degrees<double>>);
    STATIC_REQUIRE(std::is_trivially_destructible_v<swgtk::Radians<double>>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<swgtk::Degrees<float>>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<swgtk::Radians<float>>);
    STATIC_REQUIRE(std::is_trivially_assignable_v<swgtk::Radians<double>, swgtk::Radians<double>>);
    STATIC_REQUIRE(std::is_trivially_assignable_v<swgtk::Degrees<double>, swgtk::Degrees<double>>);
  }

  SECTION("Test expected cases") {
    STATIC_REQUIRE(expected1);
    STATIC_REQUIRE(expected2);
    STATIC_REQUIRE(expected3);
    STATIC_REQUIRE(expected4);
  }

  SECTION("Test out-of-bounds input") {
    STATIC_REQUIRE_FALSE(swgtk::Degrees{560.0}.value() == 560.0);
    STATIC_REQUIRE_FALSE(swgtk::Radians{-560.0}.value() == -560.0);
    STATIC_REQUIRE(swgtk::Radians{std::numbers::pi * 20.0}.value() == std::numbers::pi * 2.0);
    STATIC_REQUIRE(swgtk::Degrees{-40000.0f}.value() == -360.0f);
    STATIC_REQUIRE(outOfBounds);
    STATIC_REQUIRE(outOfBounds1);
  }

  SECTION("Test arithmetic") {
    STATIC_REQUIRE((swgtk::Rads{std::numbers::pi} + (std::numbers::pi * 0.5)) == swgtk::Rads{std::numbers::pi * 1.5});
    STATIC_REQUIRE((swgtk::Degs{540.0} - 36.0) == swgtk::Degs{324});
    STATIC_REQUIRE((swgtk::Rads{std::numbers::pi} + 40000.0) == swgtk::Rads{std::numbers::pi});
    STATIC_REQUIRE((swgtk::Degs{76.0} + 9700.0) == swgtk::Degs{76.0});
  }

  SECTION("Test comparisons") {
    STATIC_REQUIRE(swgtk::Degs{144.0} < swgtk::Degrees{540.0});
    STATIC_REQUIRE(swgtk::Rads{std::numbers::pi / 2} > swgtk::Rads{std::numbers::pi * 0.4});
  }
}
// NOLINTEND(readability-magic-numbers, *-avoid-magic-numbers)
