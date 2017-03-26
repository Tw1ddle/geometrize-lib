#include "../catch.hpp"

#include "geometrize/commonutil.h"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"

TEST_CASE("Test clamp", "[util]")
{
    SECTION("Clamp needs to actually clamp to the range")
    {
        const int min{-5};
        const int max{5};
        for(int i = -10; i < 10; i++) {
            const int clamped{geometrize::commonutil::clamp(i, min, max)};
            REQUIRE((clamped >= min && clamped <= max));
        }
    }
}

TEST_CASE("Test random range", "[util]")
{
    SECTION("Random values in a range need to actually be in the range")
    {
        const int min{-30};
        const int max{30};
        for(int i = 0; i < 10000; i++) {
            const int random{geometrize::commonutil::randomRange(min, max)};
            if(random < min || random > max) {
                FAIL("Out of range value generated");
            }
        }
    }
}

TEST_CASE("Test calculation of average color", "[core]")
{
    SECTION("Simple image")
    {
        const geometrize::rgba color{10, 50, 90, 255};
        const geometrize::Bitmap image{50, 50, color};
        REQUIRE(color == geometrize::commonutil::getAverageImageColor(image));
    }
}
