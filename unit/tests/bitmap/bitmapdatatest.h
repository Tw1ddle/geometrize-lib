#pragma once

#include "../catch.hpp"

#include "geometrize/bitmap/bitmapdata.h"
#include "geometrize/bitmap/rgba.h"

TEST_CASE("Bitmap data is created", "[bitmap]")
{
    geometrize::rgba rgba{128, 128, 128, 128};
}
