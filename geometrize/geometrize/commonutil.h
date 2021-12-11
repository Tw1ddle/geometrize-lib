#pragma once

#include <algorithm>
#include <cstdint>
#include <tuple>
#include <vector>

#include "bitmap/rgba.h"

namespace geometrize
{
class Bitmap;
class Scanline;
struct ImageRunnerShapeBoundsOptions;
}

namespace geometrize
{

namespace commonutil
{

/**
 * @brief seedRandomGenerator Seeds the (thread-local) random number generators.
 * @param seed The random seed.
 */
void seedRandomGenerator(std::uint32_t seed);

/**
 * @brief randomRange Returns a random integer in the range, inclusive. Uses thread-local random number generators under the hood.
 * To ensure deterministic shape generation that can be repeated for different seeds, this should be used for shape mutation, but nothing else.
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
 * @brief getAverageImageColor Computes the average RGB color of the pixels in the bitmap.
 * @param image The image whose average color will be calculated.
 * @return The average RGB color of the image, RGBA8888 format. Alpha is set to opaque (255).
 */
geometrize::rgba getAverageImageColor(const geometrize::Bitmap& image);

/**
 * @brief scanlinesContainTransparentPixels Returns true if the scanlines contain transparent pixels in the given image
 * @param scanlines The scanlines to check
 * @param image The image whose pixels to check
 * @param minAlpha The minimum alpha level (0-255) to consider transparent
 * @return True if the scanlines contains any transparent pixels
 */
bool scanlinesContainTransparentPixels(const std::vector<geometrize::Scanline>& scanlines, const geometrize::Bitmap& image, int minAlpha);

/**
 * @brief mapShapeBoundsToImage Maps the given shape bound percentages to the given image, returning a bounding rectangle, or the whole image if the bounds were invalid
 * @param The options to map to the image
 * @param The image to map the options around
 * @return The mapped shape bounds (xMin, yMin, xMax, yMax)
 */
std::tuple<std::int32_t, std::int32_t, std::int32_t, std::int32_t> mapShapeBoundsToImage(const geometrize::ImageRunnerShapeBoundsOptions& options, const geometrize::Bitmap& image);

}

}
