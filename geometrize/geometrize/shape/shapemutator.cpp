#include "shapemutator.h"

#include <cassert>
#include <cstdint>
#include <cmath>

#include "circle.h"
#include "ellipse.h"
#include "line.h"
#include "polyline.h"
#include "quadraticbezier.h"
#include "rectangle.h"
#include "rotatedellipse.h"
#include "rotatedrectangle.h"
#include "triangle.h"

#include "../commonutil.h"

namespace
{

float wrapMax(const float x, const float maximum)
{
    return std::fmod(maximum + std::fmod(x, maximum), maximum);
}

float wrapMinMax(const float x, const float minimum, const float maximum)
{
    return minimum + wrapMax(x - minimum, maximum - minimum);
}

}

namespace geometrize
{

void setup(geometrize::Shape& s, const std::int32_t xBound, const std::int32_t yBound)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::RECTANGLE:
        setup(static_cast<geometrize::Rectangle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        setup(static_cast<geometrize::RotatedRectangle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::TRIANGLE:
        setup(static_cast<geometrize::Triangle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::ELLIPSE:
        setup(static_cast<geometrize::Ellipse&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        setup(static_cast<geometrize::RotatedEllipse&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::CIRCLE:
        setup(static_cast<geometrize::Circle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::LINE:
        setup(static_cast<geometrize::Line&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        setup(static_cast<geometrize::QuadraticBezier&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::POLYLINE:
        setup(static_cast<geometrize::Polyline&>(s), xBound, yBound);
        break;
    default:
        assert(0 && "Bad shape type");
    }
}

void setup(geometrize::Circle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    s.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_r = geometrize::commonutil::randomRange(1, 32);
}

void setup(geometrize::Ellipse& s, const std::int32_t xBound, const std::int32_t yBound)
{
    s.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_rx = geometrize::commonutil::randomRange(1, 32);
    s.m_ry = geometrize::commonutil::randomRange(1, 32);
}

void setup(geometrize::Line& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};

    s.m_x1 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
    s.m_y1 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
    s.m_x2 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
    s.m_y2 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
}

void setup(geometrize::Polyline& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};
    for(std::int32_t i = 0; i < 4; i++) {
        const std::pair<std::int32_t, std::int32_t> point{
            geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1),
            geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1)
        };
        s.m_points.push_back(point);
    }
}

void setup(geometrize::QuadraticBezier& s, const std::int32_t xBound, const std::int32_t yBound)
{
    s.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_cx = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_cy = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_x2 = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y2 = geometrize::commonutil::randomRange(0, yBound - 1);
}

void setup(geometrize::Rectangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    s.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x1) + geometrize::commonutil::randomRange(1, 32), 0, xBound - 1);
    s.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y1) + geometrize::commonutil::randomRange(1, 32), 0, yBound - 1);
}

void setup(geometrize::RotatedEllipse& s, const std::int32_t xBound, const std::int32_t yBound)
{
    s.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_rx = geometrize::commonutil::randomRange(1, 32);
    s.m_ry = geometrize::commonutil::randomRange(1, 32);
    s.m_angle = geometrize::commonutil::randomRange(0, 360);
}

void setup(geometrize::RotatedRectangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    s.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x1) + geometrize::commonutil::randomRange(1, 32), 0, xBound);
    s.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y1) + geometrize::commonutil::randomRange(1, 32), 0, yBound);
    s.m_angle = geometrize::commonutil::randomRange(0, 360);
}

void setup(geometrize::Triangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    s.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    s.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    s.m_x2 = s.m_x1 + geometrize::commonutil::randomRange(-32, 32);
    s.m_y2 = s.m_y1 + geometrize::commonutil::randomRange(-32, 32);
    s.m_x3 = s.m_x1 + geometrize::commonutil::randomRange(-32, 32);
    s.m_y3 = s.m_y1 + geometrize::commonutil::randomRange(-32, 32);
}

void mutate(geometrize::Shape& s, const std::int32_t xBound, const std::int32_t yBound)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::RECTANGLE:
        mutate(static_cast<geometrize::Rectangle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        mutate(static_cast<geometrize::RotatedRectangle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::TRIANGLE:
        mutate(static_cast<geometrize::Triangle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::ELLIPSE:
        mutate(static_cast<geometrize::Ellipse&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        mutate(static_cast<geometrize::RotatedEllipse&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::CIRCLE:
        mutate(static_cast<geometrize::Circle&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::LINE:
        mutate(static_cast<geometrize::Line&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        mutate(static_cast<geometrize::QuadraticBezier&>(s), xBound, yBound);
        break;
    case geometrize::ShapeTypes::POLYLINE:
        mutate(static_cast<geometrize::QuadraticBezier&>(s), xBound, yBound);
        break;
    default:
        assert(0 && "Bad shape type");
    }
}

void mutate(geometrize::Circle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            s.m_x = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            s.m_y = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            s.m_r = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_r) + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Ellipse& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            s.m_x = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            s.m_y = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            s.m_rx = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_rx) + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 2:
        {
            s.m_ry = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_ry) + geometrize::commonutil::randomRange(-16, 16), 1, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Line& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};

    switch(r) {
        case 0:
        {
            s.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x1) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            s.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y1) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            s.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x2) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            s.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y2) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Polyline& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t i{geometrize::commonutil::randomRange(static_cast<std::size_t>(0), s.m_points.size() - 1)};

    std::pair<std::int32_t, std::int32_t> point{s.m_points[i]};
    point.first = geometrize::commonutil::clamp(point.first + geometrize::commonutil::randomRange(-64, 64), 0, xBound - 1);
    point.second = geometrize::commonutil::clamp(point.second + geometrize::commonutil::randomRange(-64, 64), 0, yBound - 1);

    s.m_points[i] = point;
}

void mutate(geometrize::QuadraticBezier& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            s.m_cx = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_cx) + geometrize::commonutil::randomRange(-8, 8), 0, xBound - 1);
            s.m_cy = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_cy) + geometrize::commonutil::randomRange(-8, 8), 0, yBound - 1);
            break;
        }
        case 1:
        {
            s.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x1) + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
            s.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y1) + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
        case 2:
        {
            s.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x2) + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
            s.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y2) + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Rectangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            s.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x1) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            s.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y1) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            s.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x2) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            s.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y2) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::RotatedEllipse& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 3)};
    switch(r) {
        case 0:
        {
            s.m_x = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            s.m_y = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            s.m_rx = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_rx) + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 2:
        {
            s.m_ry = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_ry) + geometrize::commonutil::randomRange(-16, 16), 1, yBound - 1);
            break;
        }
        case 3:
        {
            s.m_angle = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_angle) + geometrize::commonutil::randomRange(-16, 16), 0, 360);
            break;
        }
    }
}

void mutate(geometrize::RotatedRectangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            s.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x1) + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
            s.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y1) + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 1:
        {
            s.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x2) + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
            s.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y2) + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 2:
        {
            s.m_angle = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_angle) + geometrize::commonutil::randomRange(-4, 4), 0, 360);
            break;
        }
    }
}

void mutate(geometrize::Triangle& s, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            s.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x1) + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            s.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y1) + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
        case 1:
        {
            s.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x2) + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            s.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y2) + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
        case 2:
        {
            s.m_x3 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_x3) + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            s.m_y3 = geometrize::commonutil::clamp(static_cast<std::int32_t>(s.m_y3) + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
    }
}

void translate(geometrize::Shape& s, const float x, const float y)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::RECTANGLE:
        translate(static_cast<geometrize::Rectangle&>(s), x, y);
        break;
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        translate(static_cast<geometrize::RotatedRectangle&>(s), x, y);
        break;
    case geometrize::ShapeTypes::TRIANGLE:
        translate(static_cast<geometrize::Triangle&>(s), x, y);
        break;
    case geometrize::ShapeTypes::ELLIPSE:
        translate(static_cast<geometrize::Ellipse&>(s), x, y);
        break;
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        translate(static_cast<geometrize::RotatedEllipse&>(s), x, y);
        break;
    case geometrize::ShapeTypes::CIRCLE:
        translate(static_cast<geometrize::Circle&>(s), x, y);
        break;
    case geometrize::ShapeTypes::LINE:
        translate(static_cast<geometrize::Line&>(s), x, y);
        break;
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        translate(static_cast<geometrize::QuadraticBezier&>(s), x, y);
        break;
    case geometrize::ShapeTypes::POLYLINE:
        translate(static_cast<geometrize::QuadraticBezier&>(s), x, y);
        break;
    default:
        assert(0 && "Bad shape type");
    }
}

void translate(geometrize::Circle& s, const float x, const float y)
{
    s.m_x += x;
    s.m_y += y;
}

void translate(geometrize::Ellipse& s, const float x, const float y)
{
    s.m_x += x;
    s.m_y += y;
}

void translate(geometrize::Line& s, const float x, const float y)
{
    s.m_x1 += x;
    s.m_y1 += y;
    s.m_x2 += x;
    s.m_y2 += y;
}

void translate(geometrize::Polyline& s, const float x, const float y)
{
    for(auto& point : s.m_points) {
        point.first += x;
        point.second += y;
    }
}

void translate(geometrize::QuadraticBezier& s, const float x, const float y)
{
    s.m_cx += x;
    s.m_cy += y;
    s.m_x1 += x;
    s.m_y1 += y;
    s.m_x2 += x;
    s.m_y2 += y;
}

void translate(geometrize::Rectangle& s, const float x, const float y)
{
    s.m_x1 += x;
    s.m_y1 += y;
    s.m_x2 += x;
    s.m_y2 += y;
}

void translate(geometrize::RotatedEllipse& s, const float x, const float y)
{
    s.m_x += x;
    s.m_y += y;
}

void translate(geometrize::RotatedRectangle& s, const float x, const float y)
{
    s.m_x1 += x;
    s.m_y1 += y;
    s.m_x2 += x;
    s.m_y2 += y;
}

void translate(geometrize::Triangle& s, const float x, const float y)
{
    s.m_x1 += x;
    s.m_y1 += y;
    s.m_x2 += x;
    s.m_y2 += y;
    s.m_x3 += x;
    s.m_y3 += y;
}

void scale(geometrize::Shape& s, const float scaleFactor)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::RECTANGLE:
        scale(static_cast<geometrize::Rectangle&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        scale(static_cast<geometrize::RotatedRectangle&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::TRIANGLE:
        scale(static_cast<geometrize::Triangle&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::ELLIPSE:
        scale(static_cast<geometrize::Ellipse&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        scale(static_cast<geometrize::RotatedEllipse&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::CIRCLE:
        scale(static_cast<geometrize::Circle&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::LINE:
        scale(static_cast<geometrize::Line&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        scale(static_cast<geometrize::QuadraticBezier&>(s), scaleFactor);
        break;
    case geometrize::ShapeTypes::POLYLINE:
        scale(static_cast<geometrize::QuadraticBezier&>(s), scaleFactor);
        break;
    default:
        assert(0 && "Bad shape type");
    }
}

void scale(geometrize::Circle& s, const float scaleFactor)
{
    s.m_r *= scaleFactor;
}

void scale(geometrize::Ellipse& s, const float scaleFactor)
{
    s.m_rx *= scaleFactor;
    s.m_ry *= scaleFactor;
}

void scale(geometrize::Line& s, const float scaleFactor)
{
    const float xMid = (s.m_x1 + s.m_x2) / 2;
    const float yMid = (s.m_y1 + s.m_y2) / 2;

    s.m_x1 = (s.m_x1 - xMid) * scaleFactor + xMid;
    s.m_x2 = (s.m_x2 - xMid) * scaleFactor + xMid;

    s.m_y1 = (s.m_y1 - yMid) * scaleFactor + yMid;
    s.m_y2 = (s.m_y2 - yMid) * scaleFactor + yMid;
}

void scale(geometrize::Polyline& s, const float scaleFactor)
{
    std::vector<std::pair<float, float>> points;

    for(std::size_t i = 0; i < s.m_points.size(); i+=2) {
        if(i == s.m_points.size() - 1 || i == s.m_points.size()) {
            continue;
        }
        const float x1 = s.m_points[i].first;
        const float x2 = s.m_points[i + 1].first;
        const float y1 = s.m_points[i].second;
        const float y2 = s.m_points[i + 1].second;

        const float xLen = (x1 + x2) / 2;
        const float yLen = (y1 + y2) / 2;

        points.push_back({(x1 - xLen) * scaleFactor + xLen, (y1 - yLen) * scaleFactor + yLen});
        points.push_back({(x1 - xLen) * scaleFactor + xLen, (y2 - yLen) * scaleFactor + yLen});
    }

    s.m_points = points;
}

void scale(geometrize::QuadraticBezier& s, const float scaleFactor)
{
    assert(0 && "TODO");
}

void scale(geometrize::Rectangle& s, const float scaleFactor)
{
    const float xMid = (s.m_x1 + s.m_x2) / 2;
    const float yMid = (s.m_y1 + s.m_y2) / 2;

    s.m_x1 = (s.m_x1 - xMid) * scaleFactor + xMid;
    s.m_y1 = (s.m_y1 - yMid) * scaleFactor + yMid;
    s.m_x2 = (s.m_x2 - xMid) * scaleFactor + xMid;
    s.m_y2 = (s.m_y2 - yMid) * scaleFactor + yMid;
}

void scale(geometrize::RotatedEllipse& s, const float scaleFactor)
{
    s.m_rx *= scaleFactor;
    s.m_ry *= scaleFactor;
}

void scale(geometrize::RotatedRectangle& s, const float scaleFactor)
{
    const float xMid = (s.m_x1 + s.m_x2) / 2;
    const float yMid = (s.m_y1 + s.m_y2) / 2;

    s.m_x1 = (s.m_x1 - xMid) * scaleFactor + xMid;
    s.m_y1 = (s.m_y1 - yMid) * scaleFactor + yMid;
    s.m_x2 = (s.m_x2 - xMid) * scaleFactor + xMid;
    s.m_y2 = (s.m_y2 - yMid) * scaleFactor + yMid;
}

void scale(geometrize::Triangle& s, const float scaleFactor)
{
    const float xMid = (s.m_x1 + s.m_x2 + s.m_x3) / 3;
    const float yMid = (s.m_y1 + s.m_y2 + s.m_y3) / 3;

    s.m_x1 = (s.m_x1 - xMid) * scaleFactor + xMid;
    s.m_y1 = (s.m_y1 - yMid) * scaleFactor + yMid;
    s.m_x2 = (s.m_x2 - xMid) * scaleFactor + xMid;
    s.m_y2 = (s.m_y2 - yMid) * scaleFactor + yMid;
    s.m_x3 = (s.m_x3 - xMid) * scaleFactor + xMid;
    s.m_y3 = (s.m_y3 - yMid) * scaleFactor + yMid;
}

void rotate(geometrize::Shape& s, const float angle)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        rotate(static_cast<geometrize::RotatedRectangle&>(s), angle);
        break;
    case geometrize::ShapeTypes::TRIANGLE:
        rotate(static_cast<geometrize::Triangle&>(s), angle);
        break;
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        rotate(static_cast<geometrize::RotatedEllipse&>(s), angle);
        break;
    case geometrize::ShapeTypes::LINE:
        rotate(static_cast<geometrize::Line&>(s), angle);
        break;
    default:
        break;
    }
}

void rotate(geometrize::Line& s, const float angle)
{
    const float xMid = (s.m_x1 + s.m_x2) / 2;
    const float yMid = (s.m_y1 + s.m_y2) / 2;

    translate(s, -xMid, -yMid);

    const float x1 = s.m_x1;
    const float x2 = s.m_x2;
    const float y1 = s.m_y1;
    const float y2 = s.m_y2;
    const float cosAngle = std::cos(angle);
    const float sinAngle = std::sin(angle);

    s.m_x1 = x1 * cosAngle - y1 * sinAngle;
    s.m_y1 = x1 * sinAngle + y1 * cosAngle;
    s.m_x2 = x2 * cosAngle - y2 * sinAngle;
    s.m_y2 = x2 * sinAngle + y2 * cosAngle;

    translate(s, xMid, yMid);
}

void rotate(geometrize::RotatedEllipse& s, const float angle)
{
    s.m_angle = wrapMinMax(s.m_angle + angle, 0, 360);
}

void rotate(geometrize::RotatedRectangle& s, const float angle)
{
    s.m_angle = wrapMinMax(s.m_angle + angle, 0, 360);
}

void rotate(geometrize::Triangle& s, const float angle)
{
    const float xMid = (s.m_x1 + s.m_x2 + s.m_x3) / 3;
    const float yMid = (s.m_y1 + s.m_y2 + s.m_y3) / 3;

    translate(s, -xMid, -yMid);

    const float x1 = s.m_x1;
    const float x2 = s.m_x2;
    const float x3 = s.m_x3;
    const float y1 = s.m_y1;
    const float y2 = s.m_y2;
    const float y3 = s.m_y3;
    const float cosAngle = std::cos(angle);
    const float sinAngle = std::sin(angle);

    s.m_x1 = x1 * cosAngle - y1 * sinAngle;
    s.m_y1 = x1 * sinAngle + y1 * cosAngle;
    s.m_x2 = x2 * cosAngle - y2 * sinAngle;
    s.m_y2 = x2 * sinAngle + y2 * cosAngle;
    s.m_x3 = x3 * cosAngle - y3 * sinAngle;
    s.m_y3 = x3 * sinAngle + y3 * cosAngle;

    translate(s, xMid, yMid);
}

}
