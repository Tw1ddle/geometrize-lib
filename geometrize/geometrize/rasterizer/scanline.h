#pragma once

#include <cstdint>
#include <vector>

namespace geometrize
{

/**
 * @brief The Scanline class represents a scanline, a row of pixels running across a bitmap.
 * @author Sam Twidale (https://samcodes.co.uk/)
 */
class Scanline
{
public:
    /**
     * @brief Scanline Creates a new scanline (members are uninitialized)
     */
    Scanline() = default;

    /**
     * @brief Scanline Creates a new scanline.
     * @param y The y-coordinate.
     * @param x1 The leftmost x-coordinate.
     * @param x2 The rightmost x-coordinate.
     */
    Scanline(std::int32_t y, std::int32_t x1, std::int32_t x2);

    ~Scanline() = default;
    Scanline& operator=(const Scanline&) = default;
    Scanline(const Scanline&) = default;

    std::int32_t y; ///< The y-coordinate of the scanline.
    std::int32_t x1; ///< The leftmost x-coordinate of the scanline.
    std::int32_t x2; ///< The rightmost x-coordinate of the scanline.
};

bool operator==(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs);
bool operator!=(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs);

/**
 * @brief trimScanlines Crops the scanning width of an array of scanlines so they do not scan outside of the given area.
 * @param scanlines The scanlines to crop.
 * @param minX The minimum x value to crop to.
 * @param minY The minimum y value to crop to.
 * @param maxX The maximum x value to crop to.
 * @param maxY The maximum y value to crop to.
 * @return A new vector of cropped scanlines.
 */
std::vector<geometrize::Scanline> trimScanlines(const std::vector<geometrize::Scanline>& scanlines, std::int32_t minX, std::int32_t minY, std::int32_t maxX, std::int32_t maxY);

}
