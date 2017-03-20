#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "../scanline.h"

namespace geometrize
{

std::vector<geometrize::Scanline> scanlinesForPolygon(const std::vector<std::pair<std::int32_t, std::int32_t>>& points);

}
