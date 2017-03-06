#pragma once

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

namespace geometrize
{

namespace commonutil
{

/**
 * @brief seedRandomGenerator Seeds the (thread-local) random number generator.
 * @param seed The seed.
 */
void seedRandomGenerator(std::uint32_t seed);

/**
 * @brief randomRange Returns a random integer in the range, inclusive.
 * @param min The lower bound.
 * @param max The upper bound.
 * @return The random integer in the range.
 */
std::int32_t randomRange(std::int32_t min, std::int32_t max);

/**
 * @brief clamp Clamps a value within a range.
 * @param value The value to clamp.
 * @param lower The lower bound of the range.
 * @param upper The upper bound of the range.
 * @return The clamped value.
 */
template<typename T> T clamp(const T& value, const T& lower, const T& upper)
{
    return (std::max)(lower, (std::min)(value, upper));
}

/**
 * @brief bresenham Bresenham's line algorithm. Returns the points on the line.
 * @param x1 The start x-coordinate.
 * @param y1 The start y-coordinate.
 * @param x2 The end x-coordinate.
 * @param y The end y-coordinate.
 * @return The points on the resulting line.
 */
std::vector<std::pair<std::int32_t, std::int32_t>> bresenham(std::int32_t x1, std::int32_t y1, const std::int32_t x2, const std::int32_t y2);

}

}
