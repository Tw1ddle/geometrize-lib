#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "../bitmap/rgba.h"

namespace geometrize
{
class Bitmap;
class Circle;
class Ellipse;
class Line;
class Polyline;
class QuadraticBezier;
class Shape;
class Rectangle;
class RotatedEllipse;
class RotatedRectangle;
class Triangle;
class Scanline;
}

namespace geometrize
{

/**
 * @brief getCornerPoints Gets the corner points of the given rotated rectangle.
 * @param r The rotated rectangle.
 * @return The corner points of the rotated rectangle.
 */
std::vector<std::pair<float, float>> getCornerPoints(const geometrize::RotatedRectangle& r);

/**
 * @brief getPointsOnRotatedEllipse Calculates and returns a number of points on the given rotated ellipse.
 * @param e The rotated ellipse.
 * @return A vector containing the points on the rotated ellipse.
 */
std::vector<std::pair<float, float>> getPointsOnRotatedEllipse(const geometrize::RotatedEllipse& e, std::size_t numPoints);

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
std::vector<std::pair<std::int32_t, std::int32_t>> bresenham(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2);

/**
 * @brief scanlinesForPolygon Gets the scanlines for a series of points that make up an arbitrary polygon.
 * @param points The vertices of the polygon.
 * @return Scanlines for the polygon.
 */
std::vector<geometrize::Scanline> scanlinesForPolygon(const std::vector<std::pair<float, float>>& points);

std::vector<geometrize::Scanline> rasterize(const geometrize::Shape& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::Circle& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::Ellipse& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::Line& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::Polyline& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::QuadraticBezier& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::Rectangle& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::RotatedEllipse& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::RotatedRectangle& s, std::int32_t xBound, std::int32_t yBound);
std::vector<geometrize::Scanline> rasterize(const geometrize::Triangle& s, std::int32_t xBound, std::int32_t yBound);

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

bool shapesOverlap(const geometrize::Shape& a, const geometrize::Shape& b, std::int32_t xBound, std::int32_t yBound);
bool shapeContains(const geometrize::Shape& container, const geometrize::Shape& containee, std::int32_t xBound, std::int32_t yBound);

std::vector<std::pair<std::int32_t, std::int32_t>> shapeToPixels(const geometrize::Shape& shape, const std::uint32_t xBound, const std::uint32_t yBound);

}
