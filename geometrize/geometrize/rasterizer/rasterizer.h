#pragma once

#include <vector>

#include "../bitmap/rgba.h"

namespace geometrize
{
class Bitmap;
class Scanline;
}

namespace geometrize
{

/**
 * @brief drawLines Draws scanlines onto an image.
 * @param image The image to be drawn to.
 * @param color The color of the scanlines.
 * @param lines The scanlines to draw.
 */
void drawLines(geometrize::Bitmap& image, geometrize::rgba color, const std::vector<geometrize::Scanline>& lines);

/**
 * @brief copyLines Copies source pixels to a destination defined by a set of scanlines.
 * @param destination The destination bitmap to copy the lines to.
 * @param source The source bitmap to copy the lines from.
 * @param lines The scanlines that comprise the source to destination copying mask.
 */
void copyLines(geometrize::Bitmap& destination, const geometrize::Bitmap& source, const std::vector<geometrize::Scanline>& lines);

}
