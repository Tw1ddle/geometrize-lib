#include "rasterizer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "../commonutil.h"
#include "../bitmap/bitmap.h"
#include "../bitmap/rgba.h"
#include "../shape/circle.h"
#include "../shape/ellipse.h"
#include "../shape/line.h"
#include "../shape/polyline.h"
#include "../shape/quadraticbezier.h"
#include "../shape/rectangle.h"
#include "../shape/rotatedellipse.h"
#include "../shape/rotatedrectangle.h"
#include "../shape/triangle.h"
#include "scanline.h"

namespace geometrize
{

std::vector<std::pair<float, float>> getCornerPoints(const geometrize::RotatedRectangle& r)
{
    const float x1{(std::fmin)(r.m_x1, r.m_x2)};
    const float x2{(std::fmax)(r.m_x1, r.m_x2)};
    const float y1{(std::fmin)(r.m_y1, r.m_y2)};
    const float y2{(std::fmax)(r.m_y1, r.m_y2)};

    const float cx{(x2 + x1) / 2.0f};
    const float cy{(y2 + y1) / 2.0f};

    const float ox1{x1 - cx};
    const float ox2{x2 - cx};
    const float oy1{y1 - cy};
    const float oy2{y2 - cy};

    const float rads{r.m_angle * 3.141f / 180.0f};
    const float c{std::cos(rads)};
    const float s{std::sin(rads)};

    const std::pair<float, float> ul{ox1 * c - oy1 * s + cx, ox1 * s + oy1 * c + cy};
    const std::pair<float, float> bl{ox1 * c - oy2 * s + cx, ox1 * s + oy2 * c + cy};
    const std::pair<float, float> ur{ox2 * c - oy1 * s + cx, ox2 * s + oy1 * c + cy};
    const std::pair<float, float> br{ox2 * c - oy2 * s + cx, ox2 * s + oy2 * c + cy};

    return {ul, ur, br, bl};
}

std::vector<std::pair<float, float>> getPointsOnRotatedEllipse(const geometrize::RotatedEllipse& e, const std::size_t numPoints)
{    
    std::vector<std::pair<float, float>> points;
    const float rads{e.m_angle * (3.141f / 180.0f)};
    const float co{std::cos(rads)};
    const float si{std::sin(rads)};

    for(std::uint32_t i = 0; i < numPoints; i++) {
        const float angle{((360.0f / numPoints) * i) * (3.141f / 180.0f)};
        const float crx{e.m_rx * std::cos(angle)};
        const float cry{e.m_ry * std::sin(angle)};
        points.push_back(std::make_pair(crx * co - cry * si + e.m_x, crx * si + cry * co + e.m_y));
    }

    return points;
}

void drawLines(geometrize::Bitmap& image, const geometrize::rgba color, const std::vector<geometrize::Scanline>& lines)
{
    // Convert the non-premultiplied color to alpha-premultiplied 16-bits per channel RGBA
    // In other words, scale the rgb color components by the alpha component
    std::uint32_t sr{color.r};
    sr |= sr << 8;
    sr *= color.a;
    sr /= UINT8_MAX;
    std::uint32_t sg{color.g};
    sg |= sg << 8;
    sg *= color.a;
    sg /= UINT8_MAX;
    std::uint32_t sb{color.b};
    sb |= sb << 8;
    sb *= color.a;
    sb /= UINT8_MAX;
    std::uint32_t sa{color.a};
    sa |= sa << 8;

    const std::uint32_t m{UINT16_MAX};
    const std::uint32_t aa{(m - sa) * 257U};

    for(const geometrize::Scanline& line : lines) {
        const std::int32_t y{line.y};

        for(std::int32_t x = line.x1; x <= line.x2; x++) {
            const geometrize::rgba d(image.getPixel(x, y));

            const std::uint8_t r{static_cast<std::uint8_t>(((d.r * aa + sr * m) / m) >> 8)};
            const std::uint8_t g{static_cast<std::uint8_t>(((d.g * aa + sg * m) / m) >> 8)};
            const std::uint8_t b{static_cast<std::uint8_t>(((d.b * aa + sb * m) / m) >> 8)};
            const std::uint8_t a{static_cast<std::uint8_t>(((d.a * aa + sa * m) / m) >> 8)};

            image.setPixel(x, y, geometrize::rgba{r, g, b, a});
        }
    }
}

void copyLines(geometrize::Bitmap& destination, const geometrize::Bitmap& source, const std::vector<geometrize::Scanline>& lines)
{
    for(const geometrize::Scanline& line : lines) {
        const std::int32_t y{line.y};
        for(std::int32_t x = line.x1; x < line.x2; x++) {
            destination.setPixel(x, y, source.getPixel(x, y));
        }
    }
}

std::vector<std::pair<std::int32_t, std::int32_t>> bresenham(std::int32_t x1, std::int32_t y1, const std::int32_t x2, const std::int32_t y2)
{
    std::int32_t dx{x2 - x1};
    const std::int8_t ix{static_cast<std::int8_t>((dx > 0) - (dx < 0))};
    dx = std::abs(dx) << 1;

    std::int32_t dy{y2 - y1};
    const std::int8_t iy{static_cast<std::int8_t>((dy > 0) - (dy < 0))};
    dy = std::abs(dy) << 1;

    std::vector<std::pair<std::int32_t, std::int32_t>> points;
    points.push_back(std::make_pair(x1, y1));

    if (dx >= dy) {
        std::int32_t error(dy - (dx >> 1));
        while (x1 != x2) {
            if (error >= 0 && (error || (ix > 0))) {
                error -= dx;
                y1 += iy;
            }

            error += dy;
            x1 += ix;

            points.push_back(std::make_pair(x1, y1));
        }
    } else {
        std::int32_t error(dx - (dy >> 1));
        while (y1 != y2) {
            if (error >= 0 && (error || (iy > 0))) {
                error -= dy;
                x1 += ix;
            }

            error += dx;
            y1 += iy;

            points.push_back(std::make_pair(x1, y1));
        }
    }

    return points;
}

std::vector<geometrize::Scanline> scanlinesForPolygon(const std::vector<std::pair<float, float>>& points)
{
    std::vector<geometrize::Scanline> lines;

    std::vector<std::pair<std::int32_t, std::int32_t>> edges;
    for(std::size_t i = 0; i < points.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> p1{static_cast<std::int32_t>(points[i].first), static_cast<std::int32_t>(points[i].second)};
        const std::pair<std::int32_t, std::int32_t> p2{(i == (points.size() - 1)) ? std::make_pair(static_cast<std::int32_t>(points[0U].first), static_cast<std::int32_t>(points[0U].second)) : std::make_pair(static_cast<std::int32_t>(points[i + 1U].first), static_cast<std::int32_t>(points[i + 1U].second))};
        const std::vector<std::pair<std::int32_t, std::int32_t>> p1p2{geometrize::bresenham(p1.first, p1.second, p2.first, p2.second)};
        edges.insert(edges.end(), p1p2.begin(), p1p2.end());
    }

    std::map<std::int32_t, std::set<std::int32_t>> yToXs;
    for(std::pair<std::int32_t, std::int32_t> point : edges) {
        yToXs[point.second].insert(point.first);
    }

    for(const auto& it : yToXs) {
        const geometrize::Scanline scanline(it.first,
        *(std::min_element(it.second.begin(), it.second.end())),
        *(std::max_element(it.second.begin(), it.second.end())));
        lines.push_back(scanline);
    }

    return lines;
}

std::vector<geometrize::Scanline> rasterize(const geometrize::Shape& s, std::int32_t xBound, std::int32_t yBound)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::RECTANGLE:
        return rasterize(static_cast<const geometrize::Rectangle&>(s), xBound, yBound);
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        return rasterize(static_cast<const geometrize::RotatedRectangle&>(s), xBound, yBound);
    case geometrize::ShapeTypes::TRIANGLE:
        return rasterize(static_cast<const geometrize::Triangle&>(s), xBound, yBound);
    case geometrize::ShapeTypes::ELLIPSE:
        return rasterize(static_cast<const geometrize::Ellipse&>(s), xBound, yBound);
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        return rasterize(static_cast<const geometrize::RotatedEllipse&>(s), xBound, yBound);
    case geometrize::ShapeTypes::CIRCLE:
        return rasterize(static_cast<const geometrize::Circle&>(s), xBound, yBound);
    case geometrize::ShapeTypes::LINE:
        return rasterize(static_cast<const geometrize::Line&>(s), xBound, yBound);
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        return rasterize(static_cast<const geometrize::QuadraticBezier&>(s), xBound, yBound);
    case geometrize::ShapeTypes::POLYLINE:
        return rasterize(static_cast<const geometrize::QuadraticBezier&>(s), xBound, yBound);
    default:
        assert(0 && "Bad shape type");
        return std::vector<geometrize::Scanline>{};
    }
}

std::vector<geometrize::Scanline> rasterize(const geometrize::Circle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    std::vector<geometrize::Scanline> lines;

    const std::int32_t r{static_cast<std::int32_t>(s.m_r)};
    for(std::int32_t y = -r; y <= r; y++) {
        std::vector<std::int32_t> xScan;
        for(std::int32_t x = -r; x <= r; x++) {
            if(x * x + y * y <= r * r) {
                xScan.push_back(x);
            }
        }

        if(!xScan.empty()) {
            const std::int32_t fy{static_cast<std::int32_t>(s.m_y) + y};
            const std::int32_t x1{commonutil::clamp(static_cast<std::int32_t>(s.m_x) + xScan.front(), 0, xBound - 1)};
            const std::int32_t x2{commonutil::clamp(static_cast<std::int32_t>(s.m_x) + xScan.back(), 0, xBound - 1)};
            lines.push_back(geometrize::Scanline(fy, x1, x2));
        }
    }

    return geometrize::Scanline::trim(lines, xBound, yBound);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::Ellipse& s, const std::int32_t w, const std::int32_t h)
{
    std::vector<geometrize::Scanline> lines;

    const float aspect{static_cast<float>(s.m_rx) / static_cast<float>(s.m_ry)};

    for (std::int32_t dy = 0; dy < s.m_ry; dy++) {
        const std::int32_t y1{static_cast<std::int32_t>(s.m_y) - dy};
        const std::int32_t y2{static_cast<std::int32_t>(s.m_y) + dy};

        if ((y1 < 0 || y1 >= h) && (y2 < 0 || y2 >= h)) {
            continue;
        }

        const std::int32_t v{static_cast<std::int32_t>(std::sqrt(s.m_ry * s.m_ry - dy * dy) * aspect)};
        std::int32_t x1{static_cast<std::int32_t>(s.m_x) - v};
        std::int32_t x2{static_cast<std::int32_t>(s.m_x) + v};
        if (x1 < 0) {
            x1 = 0;
        }
        if (x2 >= w) {
            x2 = w - 1;
        }

        if (y1 >= 0 && y1 < h) {
            lines.push_back(Scanline(y1, x1, x2));
        }
        if (y2 >= 0 && y2 < h && dy > 0) {
            lines.push_back(Scanline(y2, x1, x2));
        }
    }

    return geometrize::Scanline::trim(lines, w, h);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::Line& s, const std::int32_t xBound, const std::int32_t yBound)
{
    std::vector<geometrize::Scanline> lines;

    const std::vector<std::pair<std::int32_t, std::int32_t>> points{geometrize::bresenham(static_cast<std::int32_t>(s.m_x1), static_cast<std::int32_t>(s.m_y1), static_cast<std::int32_t>(s.m_x2), static_cast<std::int32_t>(s.m_y2))};
    for(const auto& point : points) {
       lines.push_back(geometrize::Scanline(point.second, point.first, point.first));
    }

    return Scanline::trim(lines, xBound, yBound);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::Polyline& s, const std::int32_t xBound, const std::int32_t yBound)
{
    std::vector<geometrize::Scanline> lines;

    for(std::size_t i = 0; i < s.m_points.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> p0{s.m_points[i].first, s.m_points[i].second};
        const std::pair<std::int32_t, std::int32_t> p1{i < (s.m_points.size() - 1) ? std::make_pair(static_cast<std::int32_t>(s.m_points[i + 1].first), static_cast<std::int32_t>(s.m_points[i + 1].second)) : p0};

        const std::vector<std::pair<std::int32_t, std::int32_t>> points{geometrize::bresenham(p0.first, p0.second, p1.first, p1.second)};
        for(const auto& point : points) {
            lines.push_back(geometrize::Scanline(point.second, point.first, point.first));
        }
    }

    return Scanline::trim(lines, xBound, yBound);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::QuadraticBezier& s, const std::int32_t xBound, const std::int32_t yBound)
{
    std::vector<geometrize::Scanline> scanlines;

    std::vector<std::pair<std::int32_t, std::int32_t>> points;
    const std::uint32_t pointCount{20};
    for(std::uint32_t i = 0; i <= pointCount; i++) {
        const float t{static_cast<float>(i) / static_cast<float>(pointCount)};
        const float tp{1 - t};
        const std::int32_t x{static_cast<std::int32_t>(tp * (tp * s.m_x1 + (t * s.m_cx)) + t * ((tp * s.m_cx) + (t * s.m_x2)))};
        const std::int32_t y{static_cast<std::int32_t>(tp * (tp * s.m_y1 + (t * s.m_cy)) + t * ((tp * s.m_cy) + (t * s.m_y2)))};
        points.push_back(std::make_pair(x, y));
    }

    for(std::uint32_t i = 0; i < points.size() - 1; i++) {
        const std::pair<std::int32_t, std::int32_t> p0{points[i]};
        const std::pair<std::int32_t, std::int32_t> p1{points[i + 1]};

        const std::vector<std::pair<std::int32_t, std::int32_t>> points{geometrize::bresenham(static_cast<std::int32_t>(p0.first), static_cast<std::int32_t>(p0.second), static_cast<std::int32_t>(p1.first), static_cast<std::int32_t>(p1.second))};
        for(const std::pair<std::int32_t, std::int32_t>& point : points) {
            scanlines.push_back(geometrize::Scanline(point.second, point.first, point.first));
        }
    }

    return Scanline::trim(scanlines, xBound, yBound);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::Rectangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t x1{static_cast<std::int32_t>((std::fmin)(s.m_x1, s.m_x2))};
    const std::int32_t x2{static_cast<std::int32_t>((std::fmax)(s.m_x1, s.m_x2))};
    const std::int32_t y1{static_cast<std::int32_t>((std::fmin)(s.m_y1, s.m_y2))};
    const std::int32_t y2{static_cast<std::int32_t>((std::fmax)(s.m_y1, s.m_y2))};

    std::vector<geometrize::Scanline> lines;
    for(std::int32_t y = y1; y < y2; y++) {
        lines.push_back(geometrize::Scanline(y, x1, x2));
    }
    return geometrize::Scanline::trim(lines, xBound, yBound);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::RotatedEllipse& s, const std::int32_t w, const std::int32_t h)
{
    const std::uint32_t pointCount{20};
    std::vector<std::pair<float, float>> points = getPointsOnRotatedEllipse(s, pointCount);

    std::vector<geometrize::Scanline> scanlines{geometrize::scanlinesForPolygon(points)};
    return geometrize::Scanline::trim(scanlines, w, h);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::RotatedRectangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    std::vector<geometrize::Scanline> scanlines{geometrize::scanlinesForPolygon(getCornerPoints(s))};
    return geometrize::Scanline::trim(scanlines, xBound, yBound);
}

std::vector<geometrize::Scanline> rasterize(const geometrize::Triangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    std::vector<geometrize::Scanline> scanlines{geometrize::scanlinesForPolygon({{static_cast<std::int32_t>(s.m_x1), static_cast<std::int32_t>(s.m_y1)}, {static_cast<std::int32_t>(s.m_x2), static_cast<std::int32_t>(s.m_y2)}, {static_cast<std::int32_t>(s.m_x3), static_cast<std::int32_t>(s.m_y3)}})};
    return Scanline::trim(scanlines, xBound, yBound);
}

bool scanlinesOverlap(const std::vector<geometrize::Scanline>& first, const std::vector<geometrize::Scanline>& second)
{
    for(const auto& f : first) {
        for(const auto& s : second) {
            if(f.y == s.y) {
                if(f.x1 >= s.x1 && f.x2 <= s.x2) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool scanlinesContain(const std::vector<geometrize::Scanline>& first, const std::vector<geometrize::Scanline>& second)
{
    for(const auto& s : first) {
        bool contained = false;
        for(const auto& f : second) {
            if(f.y == s.y) {
                if(f.x1 <= s.x1 && f.x2 >= s.x2) {
                    contained = true;
                }
            }
        }

        if(!contained) {
            return false;
        }
    }

    return true;
}

bool shapesOverlap(const geometrize::Shape& a, const geometrize::Shape& b, const std::int32_t xBound, const std::int32_t yBound)
{
    return geometrize::scanlinesOverlap(geometrize::rasterize(a, xBound, yBound), geometrize::rasterize(b, xBound, yBound));
}

bool shapeContains(const geometrize::Shape& container, const geometrize::Shape& containee, const std::int32_t xBound, const std::int32_t yBound)
{
    return geometrize::scanlinesContain(rasterize(container, xBound, yBound), rasterize(containee, xBound, yBound));
}

std::vector<std::pair<std::int32_t, std::int32_t>> shapeToPixels(const geometrize::Shape& shape, const std::uint32_t xBound, const std::uint32_t yBound)
{
    const auto scanlines = geometrize::rasterize(shape, static_cast<std::int32_t>(xBound), static_cast<std::int32_t>(yBound));
    std::vector<std::pair<std::int32_t, std::int32_t>> points = {};
    for(const auto& scanline : scanlines) {
        for(std::int32_t x = scanline.x1; x < scanline.x2; x++) {
            points.push_back({x, scanline.y});
        }
    }
    return points;
}

}
