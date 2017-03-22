#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "../scanline.h"

namespace geometrize
{

/**
 * @brief bresenham Bresenham's line algorithm. Returns the points on the line.
 * @param x1 The start x-coordinate.
 * @param y1 The start y-coordinate.
 * @param x2 The end x-coordinate.
 * @param y The end y-coordinate.
 * @return The points on the resulting line.
 */
std::vector<std::pair<std::int32_t, std::int32_t>> bresenham(std::int32_t x1, std::int32_t y1, const std::int32_t x2, const std::int32_t y2);

/**
 * @brief scanlinesForPolygon Gets the scanlines for a series of points that make up an arbitrary polygon.
 * @param points The vertices of the polygon.
 * @return Scanlines for the polygon.
 */
std::vector<geometrize::Scanline> scanlinesForPolygon(const std::vector<std::pair<std::int32_t, std::int32_t>>& points);

}
