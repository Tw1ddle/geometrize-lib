#pragma once

#include <algorithm>
#include <cstdint>

namespace geometrize
{

namespace commonutil
{

/**
 * @brief randomRange Returns a random integer in the range, inclusive.
 * @param min The lower bound.
 * @param max The upper bound.
 * @return The random integer in the range.
 */
std::int32_t randomRange(std::int32_t min, std::int32_t max);

/**
 * Clamps a value within a range.
 * @param value The value to clamp.
 * @param lower The lower bound of the range.
 * @param upper The upper bound of the range.
 * @return The clamped value.
 */
template<typename T> T clamp(const T& value, const T& lower, const T& upper)
{
    return (std::max)(lower, (std::min)(value, upper));
}

}

}
