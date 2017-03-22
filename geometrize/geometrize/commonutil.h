#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "bitmap/rgba.h"

namespace geometrize
{
class Bitmap;
}

namespace geometrize
{

namespace commonutil
{

/**
 * @brief seedRandomGenerator Seeds the (thread-local) random number generator.
 * @param seed The random seed.
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
 * @brief getAverageImageColor Computes the average RGB color of the pixels in the image.
 * @param image The image whose average color will be calculated.
 * @return The average RGB color of the image, RGBA8888 format. Alpha is set to opaque (255).
 */
geometrize::rgba getAverageImageColor(const geometrize::Bitmap& image);

}

}
