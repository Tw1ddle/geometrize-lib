#include "scanline.h"

#include <cstdint>
#include <vector>

#include "../commonutil.h"

namespace geometrize
{

Scanline::Scanline(const std::int32_t y, const std::int32_t x1, const std::int32_t x2) : y{y}, x1{x1}, x2{x2} {}

std::vector<geometrize::Scanline> Scanline::trim(std::vector<geometrize::Scanline>& scanlines, const std::uint32_t w, const std::uint32_t h)
{
    std::vector<geometrize::Scanline> trimmedScanlines;

    for(geometrize::Scanline& line : scanlines) {
        if(line.y < 0 || line.y >= static_cast<std::int32_t>(h) || line.x1 >= static_cast<std::int32_t>(w) || line.x2 < 0) {
            continue;
        }
        line.x1 = geometrize::commonutil::clamp(line.x1, 0, static_cast<std::int32_t>(w) - 1);
        line.x2 = geometrize::commonutil::clamp(line.x2, 0, static_cast<std::int32_t>(w) - 1);
        if(line.x1 > line.x2) {
            continue;
        }
        trimmedScanlines.push_back(line);
    }

    return trimmedScanlines;
}

bool operator==(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs)
{
    return lhs.y == rhs.y && lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2;
}

bool operator!=(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs)
{
    return lhs.y != rhs.y || lhs.x1 != rhs.x1 || lhs.x2 != rhs.x2;
}

}
