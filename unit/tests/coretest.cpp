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

TEST_CASE("Test drawing of scanlines", "[core]")
{
    geometrize::Bitmap image(50, 50, geometrize::rgba{10, 20, 30, 128});
    std::vector<geometrize::Scanline> lines;
    for(int y = 0; y < 50; y++) {
        lines.push_back(geometrize::Scanline(y, 0, 50));
    }

    // TODO check drawn lines
}

TEST_CASE("Test copying of scanlines", "[core]")
{
    const geometrize::rgba color{128, 64, 32, 200};
    const unsigned int width{10};
    const unsigned int height{10};

    geometrize::Bitmap source{width, height, color};
    geometrize::Bitmap destination(width, height, geometrize::rgba{0, 0, 0, 0});

    std::vector<geometrize::Scanline> scanlines;
    for(std::int32_t y = 3; y < 7; y++) {
        scanlines.push_back(geometrize::Scanline(y, 3, 7));
    }

    geometrize::core::copyLines(destination, source, scanlines);

    for(const geometrize::Scanline& line : scanlines) {
        const std::int32_t y{line.y};
        for(std::int32_t x = line.x1; x < line.x2; x++) {
            REQUIRE(destination.getPixel(x, y) == color);
        }
    }
}
