#include "shapeserializer.h"

#include <cassert>
#include <cmath>
#include <vector>

#include "../shape/circle.h"
#include "../shape/ellipse.h"
#include "../shape/line.h"
#include "../shape/polyline.h"
#include "../shape/quadraticbezier.h"
#include "../shape/rectangle.h"
#include "../shape/rotatedellipse.h"
#include "../shape/rotatedrectangle.h"
#include "../shape/shape.h"
#include "../shape/triangle.h"

namespace geometrize
{

std::vector<float> getRawShapeData(const geometrize::Shape& s)
{
    switch(s.getType()) {
    case geometrize::ShapeTypes::RECTANGLE:
        return getRawShapeData(static_cast<const geometrize::Rectangle&>(s));
    case geometrize::ShapeTypes::ROTATED_RECTANGLE:
        return getRawShapeData(static_cast<const geometrize::RotatedRectangle&>(s));
    case geometrize::ShapeTypes::TRIANGLE:
        return getRawShapeData(static_cast<const geometrize::Triangle&>(s));
    case geometrize::ShapeTypes::ELLIPSE:
        return getRawShapeData(static_cast<const geometrize::Ellipse&>(s));
    case geometrize::ShapeTypes::ROTATED_ELLIPSE:
        return getRawShapeData(static_cast<const geometrize::RotatedEllipse&>(s));
    case geometrize::ShapeTypes::CIRCLE:
        return getRawShapeData(static_cast<const geometrize::Circle&>(s));
    case geometrize::ShapeTypes::LINE:
        return getRawShapeData(static_cast<const geometrize::Line&>(s));
    case geometrize::ShapeTypes::QUADRATIC_BEZIER:
        return getRawShapeData(static_cast<const geometrize::QuadraticBezier&>(s));
    case geometrize::ShapeTypes::POLYLINE:
        return getRawShapeData(static_cast<const geometrize::Polyline&>(s));
    default:
        assert(0 && "Bad shape type");
        return {};
    }
}

std::vector<float> getRawShapeData(const geometrize::Circle& s)
{
    return { s.m_x, s.m_y, s.m_r };
}

std::vector<float> getRawShapeData(const geometrize::Ellipse& s)
{
    return { s.m_x, s.m_y, s.m_rx, s.m_ry };
}

std::vector<float> getRawShapeData(const geometrize::Line& s)
{
    return { s.m_x1, s.m_y1, s.m_x2, s.m_y2 };
}

std::vector<float> getRawShapeData(const geometrize::Polyline& s)
{
    std::vector<float> data;
    for(std::size_t i = 0; i < s.m_points.size(); i++) {
        data.push_back(s.m_points[i].first);
        data.push_back(s.m_points[i].second);
    }

    return data;
}

std::vector<float> getRawShapeData(const geometrize::QuadraticBezier& s)
{
    return { s.m_x1, s.m_y1, s.m_cx, s.m_cy, s.m_x2, s.m_y2 };
}

std::vector<float> getRawShapeData(const geometrize::Rectangle& s)
{
    return {
        ((std::fmin)(s.m_x1, s.m_x2)),
        ((std::fmin)(s.m_y1, s.m_y2)),
        ((std::fmax)(s.m_x1, s.m_x2)),
        ((std::fmax)(s.m_y1, s.m_y2))
    };
}

std::vector<float> getRawShapeData(const geometrize::RotatedEllipse& s)
{
    return { s.m_x, s.m_y, s.m_rx, s.m_ry, s.m_angle };
}

std::vector<float> getRawShapeData(const geometrize::RotatedRectangle& s)
{
    return {
        ((std::fmin)(s.m_x1, s.m_x2)),
        ((std::fmin)(s.m_y1, s.m_y2)),
        ((std::fmax)(s.m_x1, s.m_x2)),
        ((std::fmax)(s.m_y1, s.m_y2)),
        s.m_angle
    };
}

std::vector<float> getRawShapeData(const geometrize::Triangle& s)
{
    return { s.m_x1, s.m_y1, s.m_x2, s.m_y2, s.m_x3, s.m_y3 };
}

}
