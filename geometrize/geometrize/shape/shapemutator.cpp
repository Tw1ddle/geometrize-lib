#include "shapemutator.h"

#include <cassert>
#include <cstdint>

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
        setup(static_cast<geometrize::QuadraticBezier&>(s), xBound, yBound);
        break;
    default:
        assert(0 && "Bad shape type");
    }
}

void setup(geometrize::Circle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_r = geometrize::commonutil::randomRange(1, 32);
}

void setup(geometrize::Ellipse& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_rx = geometrize::commonutil::randomRange(1, 32);
    shape.m_ry = geometrize::commonutil::randomRange(1, 32);
}

void setup(geometrize::Line& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};

    shape.m_x1 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
    shape.m_y2 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
}

void setup(geometrize::Polyline& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};
    for(std::int32_t i = 0; i < 4; i++) {
        const std::pair<std::int32_t, std::int32_t> point{
            geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1),
            geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1)
        };
        shape.m_points.push_back(point);
    }
}

void setup(geometrize::QuadraticBezier& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_cx = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_cy = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y2 = geometrize::commonutil::randomRange(0, yBound - 1);
}

void setup(geometrize::Rectangle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x1) + geometrize::commonutil::randomRange(1, 32), 0, xBound - 1);
    shape.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y1) + geometrize::commonutil::randomRange(1, 32), 0, yBound - 1);
}

void setup(geometrize::RotatedEllipse& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_rx = geometrize::commonutil::randomRange(1, 32);
    shape.m_ry = geometrize::commonutil::randomRange(1, 32);
    shape.m_angle = geometrize::commonutil::randomRange(0, 360);
}

void setup(geometrize::RotatedRectangle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x1) + geometrize::commonutil::randomRange(1, 32), 0, xBound);
    shape.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y1) + geometrize::commonutil::randomRange(1, 32), 0, yBound);
    shape.m_angle = geometrize::commonutil::randomRange(0, 360);
}

void setup(geometrize::Triangle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = shape.m_x1 + geometrize::commonutil::randomRange(-32, 32);
    shape.m_y2 = shape.m_y1 + geometrize::commonutil::randomRange(-32, 32);
    shape.m_x3 = shape.m_x1 + geometrize::commonutil::randomRange(-32, 32);
    shape.m_y3 = shape.m_y1 + geometrize::commonutil::randomRange(-32, 32);
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

void mutate(geometrize::Circle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            shape.m_x = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_r = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_r) + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Ellipse& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_x = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_rx = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_rx) + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 2:
        {
            shape.m_ry = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_ry) + geometrize::commonutil::randomRange(-16, 16), 1, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Line& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};

    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x1) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y1) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x2) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y2) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Polyline& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t i{geometrize::commonutil::randomRange(static_cast<std::size_t>(0), shape.m_points.size() - 1)};

    std::pair<std::int32_t, std::int32_t> point{shape.m_points[i]};
    point.first = geometrize::commonutil::clamp(point.first + geometrize::commonutil::randomRange(-64, 64), 0, xBound - 1);
    point.second = geometrize::commonutil::clamp(point.second + geometrize::commonutil::randomRange(-64, 64), 0, yBound - 1);

    shape.m_points[i] = point;
}

void mutate(geometrize::QuadraticBezier& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_cx = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_cx) + geometrize::commonutil::randomRange(-8, 8), 0, xBound - 1);
            shape.m_cy = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_cy) + geometrize::commonutil::randomRange(-8, 8), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x1) + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
            shape.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y1) + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
        case 2:
        {
            shape.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x2) + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y2) + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::Rectangle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x1) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y1) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x2) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y2) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
    }
}

void mutate(geometrize::RotatedEllipse& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 3)};
    switch(r) {
        case 0:
        {
            shape.m_x = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x) + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y) + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_rx = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_rx) + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 2:
        {
            shape.m_ry = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_ry) + geometrize::commonutil::randomRange(-16, 16), 1, yBound - 1);
            break;
        }
        case 3:
        {
            shape.m_angle = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_angle) + geometrize::commonutil::randomRange(-16, 16), 0, 360);
            break;
        }
    }
}

void mutate(geometrize::RotatedRectangle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x1) + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
            shape.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y1) + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x2) + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
            shape.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y2) + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 2:
        {
            shape.m_angle = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_angle) + geometrize::commonutil::randomRange(-4, 4), 0, 360);
            break;
        }
    }
}

void mutate(geometrize::Triangle& shape, const std::int32_t xBound, const std::int32_t yBound)
{
    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x1) + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            shape.m_y1 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y1) + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x2) + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            shape.m_y2 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y2) + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
        case 2:
        {
            shape.m_x3 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_x3) + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            shape.m_y3 = geometrize::commonutil::clamp(static_cast<std::int32_t>(shape.m_y3) + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
    }
}

}
