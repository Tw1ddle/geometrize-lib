#pragma once

#include <cstdint>
#include <vector>

namespace geometrize
{

/**
 * @brief The Scanline class represents a scanline, a row of pixels running across a bitmap.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Scanline
{
public:
    /**
     * @brief Scanline Creates a new scanline.
     * @param y The y-coordinate.
     * @param x1 The leftmost x-coordinate.
     * @param x2 The rightmost x-coordinate.
     * @param alpha The alpha transparency of the scanline.
     */
    Scanline(const std::uint32_t y, const std::uint32_t x1, const std::uint32_t x2, const std::uint32_t alpha);

    ~Scanline() = default;
    Scanline& operator=(const Scanline&) = default;
    Scanline(const Scanline&) = default;

    /**
     * @brief trim Crops the scanning width of an array of scanlines so they do not scan outside of the given area.
     * @param scanlines The scanlines to crop.
     * @param w The width to crop.
     * @param h The height to crop.
     * @return A new vector of cropped scanlines.
     */
    static std::vector<geometrize::Scanline> trim(std::vector<geometrize::Scanline>& scanlines, const std::uint32_t w, const std::uint32_t h);

    const std::uint32_t y; ///< The y-coordinate of the scanline.
    std::uint32_t x1; ///< The leftmost x-coordinate of the scanline.
    std::uint32_t x2; ///< The rightmost x-coordinate of the scanline.
    const std::uint32_t alpha; ///< The alpha/opacity of the scanline.
};

inline bool operator==(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs)
{
    return lhs.y == rhs.y && lhs.x1 == rhs.x1 && lhs.x2 == rhs.x2 && lhs.alpha == rhs.alpha;
}

inline bool operator!=(const geometrize::Scanline& lhs, const geometrize::Scanline& rhs)
{
    return lhs.y != rhs.y || lhs.x1 != rhs.x1 || lhs.x2 != rhs.x2 || lhs.alpha != rhs.alpha;
}

}
