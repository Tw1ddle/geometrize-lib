#pragma once

#include <cstdint>
#include <utility>
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

/**
 * @brief bresenham Bresenham's line algorithm. Returns the points on the line.
 * @param x1 The start x-coordinate.
 * @param y1 The start y-coordinate.
 * @param x2 The end x-coordinate.
 * @param y2 The end y-coordinate.
 * @return The points on the resulting line.
 */
std::vector<std::pair<std::int32_t, std::int32_t>> bresenham(std::int32_t x1, std::int32_t y1, const std::int32_t x2, const std::int32_t y2);

/**
 * @brief scanlinesForPolygon Gets the scanlines for a series of points that make up an arbitrary polygon.
 * @param points The vertices of the polygon.
 * @return Scanlines for the polygon.
 */
std::vector<geometrize::Scanline> scanlinesForPolygon(const std::vector<std::pair<float, float>>& points);

/**
 * @brief scanlinesOverlap Returns true if any of the scanlines from the first vector overlap the second
 * @param first First collection of scanlines.
 * @param second Second collection of scanlines.
 * @return True if there are any overlaps, else false.
 */
bool scanlinesOverlap(const std::vector<geometrize::Scanline>& first, const std::vector<geometrize::Scanline>& second);

/**
 * @brief scanlinesContain Returns true if the first vector of scanlines wholly contain the second vector of scanlines.
 * @param first First collection of scanlines.
 * @param second Second collection of scanlines.
 * @return True if the first set of scanlines wholly contains the second set, else false.
 */
bool scanlinesContain(const std::vector<geometrize::Scanline>& first, const std::vector<geometrize::Scanline>& second);

}
