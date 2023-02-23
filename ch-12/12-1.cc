#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstdio>
#include <optional>

template <typename To, typename From>
std::optional<To> narrow_cast(From value)
{
    const auto converted = static_cast<To>(value);
    const auto backwards = static_cast<From>(converted);
    if(value != backwards)
        return std::nullopt;
    return converted;
}

TEST_CASE("Not Narrowed")
{
    int perfect { 496 };
    const auto perfect_short{ narrow_cast<short>(perfect) };
    REQUIRE(perfect_short.has_value());
    REQUIRE(perfect_short == perfect);
}

TEST_CASE("Narrowed")
{
    int perfect { 142857 };
    const auto perfect_short{ narrow_cast<short>(perfect) };
    REQUIRE_FALSE(perfect_short.has_value());
}
