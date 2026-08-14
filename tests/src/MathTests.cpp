#include <catch2/catch_test_macros.hpp>
#include <swgtk/Math.hpp>

// NOLINTBEGIN(readability-magic-numbers, *-avoid-magic-numbers, *-avoid-do-while, readability-function-cognitive-complexity, misc-use-anonymous-namespace)

// Attempting to convince msvc to test the helper functions at compile-time

namespace {
  constexpr auto expected1 = swgtk::degrees_to_radians(swgtk::degrees{90.0f}) == swgtk::radians{std::numbers::pi_v<float> * 0.5f};
  constexpr auto expected2 = swgtk::degrees_to_radians(swgtk::degrees{180.0}) == swgtk::radians{std::numbers::pi};
  constexpr auto expected3 = swgtk::radians_to_degrees(swgtk::radians{std::numbers::pi_v<float> * 0.25f}) == swgtk::degrees{45.0f};
  constexpr auto expected4 = swgtk::radians_to_degrees(swgtk::radians{std::numbers::pi * 2.0}) == swgtk::degrees{360.0};
  constexpr auto out_of_bounds = [] consteval -> bool { return swgtk::degrees_to_radians(swgtk::degs{540.0} - 45.0) == swgtk::rads{ std::numbers::pi * 1.75 }; }();
  constexpr auto out_of_bounds1 = [] consteval -> bool { return swgtk::radians_to_degrees(swgtk::rads{ std::numbers::pi } + (std::numbers::pi * 30.0)) == swgtk::degs{ 180.0 }; }();
}

// Suppress irrelevant warnings that prevent Catch2 macros from compiling.
#pragma clang diagnostic ignored "-Wc2y-extensions"
#pragma GCC diagnostic ignored "-Wc2y-extensions"

TEST_CASE("Angle Tests") {
  SECTION("Test type traits") {
    STATIC_REQUIRE(std::is_trivially_destructible_v<swgtk::degrees<double>>);
    STATIC_REQUIRE(std::is_trivially_destructible_v<swgtk::radians<double>>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<swgtk::degrees<float>>);
    STATIC_REQUIRE(std::is_trivially_copyable_v<swgtk::radians<float>>);
    STATIC_REQUIRE(std::is_trivially_assignable_v<swgtk::radians<double>, swgtk::radians<double>>);
    STATIC_REQUIRE(std::is_trivially_assignable_v<swgtk::degrees<double>, swgtk::degrees<double>>);
  }

  SECTION("Test expected cases") {
    STATIC_REQUIRE(expected1);
    STATIC_REQUIRE(expected2);
    STATIC_REQUIRE(expected3);
    STATIC_REQUIRE(expected4);
  }

  SECTION("Test out-of-bounds input") {
    STATIC_REQUIRE_FALSE(swgtk::degrees{560.0}.value() == 560.0);
    STATIC_REQUIRE_FALSE(swgtk::radians{-560.0}.value() == -560.0);
    STATIC_REQUIRE(swgtk::radians{std::numbers::pi * 20.0}.value() == std::numbers::pi * 2.0);
    STATIC_REQUIRE(swgtk::degrees{-40000.0f}.value() == -360.0f);
    STATIC_REQUIRE(out_of_bounds);
    STATIC_REQUIRE(out_of_bounds1);
  }

  SECTION("Test arithmetic") {
    STATIC_REQUIRE((swgtk::rads{std::numbers::pi} + (std::numbers::pi * 0.5)) == swgtk::rads{std::numbers::pi * 1.5});
    STATIC_REQUIRE((swgtk::degs{540.0} - 36.0) == swgtk::degs{324});
    STATIC_REQUIRE((swgtk::rads{std::numbers::pi} + 40000.0) == swgtk::rads{std::numbers::pi});
    STATIC_REQUIRE((swgtk::degs{76.0} + 9700.0) == swgtk::degs{76.0});
  }

  SECTION("Test comparisons") {
    STATIC_REQUIRE(swgtk::degs{144.0} < swgtk::degrees{540.0});
    STATIC_REQUIRE(swgtk::rads{std::numbers::pi / 2} > swgtk::rads{std::numbers::pi * 0.4});
  }
}
// NOLINTEND(readability-magic-numbers, *-avoid-magic-numbers, *-avoid-do-while, readability-function-cognitive-complexity, misc-use-anonymous-namespace)
