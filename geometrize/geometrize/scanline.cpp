#include "scanline.h"

#include <cstdint>
#include <vector>

#include "commonutil.h"

namespace geometrize
{

/**
 * @brief Scanline Creates a new scanline.
 * @param y The y-coordinate.
 * @param x1 The leftmost x-coordinate.
 * @param x2 The rightmost x-coordinate.
 * @param alpha The alpha transparency of the scanline.
 */
Scanline::Scanline(const std::uint32_t y, const std::uint32_t x1, const std::uint32_t x2, const std::uint32_t alpha) : y{y}, x1{x1}, x2{x2}, alpha{alpha} {}

/**
 * @brief trim Crops the scanning width of an array of scanlines so they do not scan outside of the given area.
 * @param scanlines The scanlines to crop.
 * @param w The width to crop.
 * @param h The height to crop.
 * @return A new vector of cropped scanlines.
 */
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

}
