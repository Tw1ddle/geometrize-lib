#include "scanline.h"

#include <cstdint>
#include <vector>

#include "commonutil.h"

namespace geometrize
{

Scanline::Scanline(const std::uint32_t y, const std::uint32_t x1, const std::uint32_t x2, const std::uint32_t alpha) : y{y}, x1{x1}, x2{x2}, alpha{alpha} {}

std::vector<geometrize::Scanline> Scanline::trim(std::vector<geometrize::Scanline>& scanlines, const std::uint32_t w, const std::uint32_t h)
{
    std::vector<geometrize::Scanline> trimmedScanlines;

    for(geometrize::Scanline& line : scanlines) {
        if(line.y >= h || line.x1 >= w) {
            continue;
        }
        line.x1 = geometrize::commonutil::clamp(line.x1, 0U, w - 1);
        line.x2 = geometrize::commonutil::clamp(line.x2, 0U, w - 1);
        if(line.x1 > line.x2) {
            continue;
        }
        trimmedScanlines.push_back(line);
    }

    return trimmedScanlines;
}

bool operator==(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs)
{
    return lhs.y == rhs.y && lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2 && lhs.alpha == rhs.alpha;
}

bool operator!=(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs)
{
    return lhs.y != rhs.y || lhs.x1 != rhs.x1 || lhs.x2 != rhs.x2 || lhs.alpha != rhs.alpha;
}

}
