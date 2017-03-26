#include "../catch.hpp"

#include <cstdint>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/core.h"

geometrize::rgba computeScanlinesColor(const geometrize::rgba targetColor, const geometrize::rgba currentColor, const int alpha)
{
    geometrize::Bitmap target(10, 10, targetColor);
    geometrize::Bitmap current(10, 10, currentColor);
    std::vector<geometrize::Scanline> lines;
    for(int y = 0; y < 10; y++) {
        lines.push_back(geometrize::Scanline(y, 0, 9));
    }
    return geometrize::core::computeColor(target, current, lines, alpha);
}

TEST_CASE("Test computation of scanline color for whole image", "[core]")
{
    const geometrize::rgba expected{23, 0, 0, 128};
    const geometrize::rgba result{computeScanlinesColor(geometrize::rgba{12, 12, 12, 0}, geometrize::rgba{0, 212, 62, 0}, 128)};
    REQUIRE(expected == result);
}
