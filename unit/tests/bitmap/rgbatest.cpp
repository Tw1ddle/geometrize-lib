#include "../catch.hpp"

#include "geometrize/bitmap/rgba.h"

TEST_CASE("Rgba creation check", "[rgba]")
{
    SECTION("Rgba creation")
    {
        const geometrize::rgba color{64, 128, 255, 0};
        REQUIRE(color.r == 64);
        REQUIRE(color.g == 128);
        REQUIRE(color.b == 255);
        REQUIRE(color.a == 0);
    }
}
