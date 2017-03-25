#include "../catch.hpp"

#include "geometrize/rasterizer/scanline.h"

#include <vector>

TEST_CASE("Test scanline creation", "[scanline]")
{
    geometrize::Scanline scanline(50, 10, 20);
    REQUIRE(scanline.y == 50);
    REQUIRE(scanline.x1 == 10);
    REQUIRE(scanline.x2 == 20);
}

TEST_CASE("Test trimming scanlines", "[scanline]")
{
    std::vector<geometrize::Scanline> scanlines;
    for(int i = 0; i < 20; i++) {
        scanlines.push_back(geometrize::Scanline(i, i, i * 2));
    }
    std::vector<geometrize::Scanline> trimmedLines = geometrize::Scanline::trim(scanlines, 100, 100);

    REQUIRE(scanlines.size() == trimmedLines.size());
    for(int j = 0; j < scanlines.size(); j++) {
        REQUIRE((scanlines[j] == trimmedLines[j]));
    }
}
