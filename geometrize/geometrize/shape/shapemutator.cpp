#include "shapemutator.h"

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
#include "../model.h"

namespace geometrize
{

void setupCircle(geometrize::Circle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_r = geometrize::commonutil::randomRange(1, 32);
}

void setupEllipse(geometrize::Ellipse& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_rx = geometrize::commonutil::randomRange(1, 32);
    shape.m_ry = geometrize::commonutil::randomRange(1, 32);
}

void setupLine(geometrize::Line& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};

    shape.m_x1 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1);
    shape.m_y2 = geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1);
}

void setupPolyline(geometrize::Polyline& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(geometrize::commonutil::randomRange(0, xBound), geometrize::commonutil::randomRange(0, yBound - 1))};
    for(std::int32_t i = 0; i < 4; i++) {
        const std::pair<std::int32_t, std::int32_t> point{
            geometrize::commonutil::clamp(startingPoint.first + geometrize::commonutil::randomRange(-32, 32), 0, xBound - 1),
            geometrize::commonutil::clamp(startingPoint.second + geometrize::commonutil::randomRange(-32, 32), 0, yBound - 1)
        };
        shape.m_points.push_back(point);
    }
}

void setupQuadraticBezier(geometrize::QuadraticBezier& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_cx = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_cy = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y2 = geometrize::commonutil::randomRange(0, yBound - 1);
}

void setupRectangle(geometrize::Rectangle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(1, 32), 0, xBound - 1);
    shape.m_y2 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(1, 32), 0, yBound - 1);
}

void setupRotatedEllipse(geometrize::RotatedEllipse& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    shape.m_x = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_rx = geometrize::commonutil::randomRange(1, 32);
    shape.m_ry = geometrize::commonutil::randomRange(1, 32);
    shape.m_angle = geometrize::commonutil::randomRange(0, 360);
}

void setupRotatedRectangle(geometrize::RotatedRectangle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(1, 32), 0, xBound);
    shape.m_y2 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(1, 32), 0, yBound);
    shape.m_angle = geometrize::commonutil::randomRange(0, 360);
}

void setupTriangle(geometrize::Triangle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    shape.m_x1 = geometrize::commonutil::randomRange(0, xBound - 1);
    shape.m_y1 = geometrize::commonutil::randomRange(0, yBound - 1);
    shape.m_x2 = shape.m_x1 + geometrize::commonutil::randomRange(-32, 32);
    shape.m_y2 = shape.m_y1 + geometrize::commonutil::randomRange(-32, 32);
    shape.m_x3 = shape.m_x1 + geometrize::commonutil::randomRange(-32, 32);
    shape.m_y3 = shape.m_y1 + geometrize::commonutil::randomRange(-32, 32);
}

void mutateCircle(geometrize::Circle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            shape.m_x = geometrize::commonutil::clamp(shape.m_x + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y = geometrize::commonutil::clamp(shape.m_y + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_r = geometrize::commonutil::clamp(shape.m_r + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
    }
}

void mutateEllipse(geometrize::Ellipse& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_x = geometrize::commonutil::clamp(shape.m_x + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y = geometrize::commonutil::clamp(shape.m_y + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_rx = geometrize::commonutil::clamp(shape.m_rx + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 2:
        {
            shape.m_ry = geometrize::commonutil::clamp(shape.m_ry + geometrize::commonutil::randomRange(-16, 16), 1, yBound - 1);
            break;
        }
    }
}

void mutateLine(geometrize::Line& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};

    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
    }
}

void mutatePolyline(geometrize::Polyline& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};
    const std::int32_t i{geometrize::commonutil::randomRange(static_cast<std::size_t>(0), shape.m_points.size() - 1)};

    std::pair<std::int32_t, std::int32_t> point{shape.m_points[i]};
    point.first = geometrize::commonutil::clamp(point.first + geometrize::commonutil::randomRange(-64, 64), 0, xBound - 1);
    point.second = geometrize::commonutil::clamp(point.second + geometrize::commonutil::randomRange(-64, 64), 0, yBound - 1);

    shape.m_points[i] = point;
}

void mutateQuadraticBezier(geometrize::QuadraticBezier& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_cx = geometrize::commonutil::clamp(shape.m_cx + geometrize::commonutil::randomRange(-8, 8), 0, xBound - 1);
            shape.m_cy = geometrize::commonutil::clamp(shape.m_cy + geometrize::commonutil::randomRange(-8, 8), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
            shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
        case 2:
        {
            shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-8, 8), 1, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
    }
}

void mutateRectangle(geometrize::Rectangle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
    }
}

void mutateRotatedEllipse(geometrize::RotatedEllipse& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 3)};
    switch(r) {
        case 0:
        {
            shape.m_x = geometrize::commonutil::clamp(shape.m_x + geometrize::commonutil::randomRange(-16, 16), 0, xBound - 1);
            shape.m_y = geometrize::commonutil::clamp(shape.m_y + geometrize::commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            shape.m_rx = geometrize::commonutil::clamp(shape.m_rx + geometrize::commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 2:
        {
            shape.m_ry = geometrize::commonutil::clamp(shape.m_ry + geometrize::commonutil::randomRange(-16, 16), 1, yBound - 1);
            break;
        }
        case 3:
        {
            shape.m_angle = geometrize::commonutil::clamp(shape.m_angle + geometrize::commonutil::randomRange(-16, 16), 0, 360);
            break;
        }
    }
}

void mutateRotatedRectangle(geometrize::RotatedRectangle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
            shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-16, 16), 0, xBound);
            shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 2:
        {
            shape.m_angle = geometrize::commonutil::clamp(shape.m_angle + geometrize::commonutil::randomRange(-4, 4), 0, 360);
            break;
        }
    }
}

void mutateTriangle(geometrize::Triangle& shape)
{
    const std::int32_t xBound{shape.m_model->getWidth()};
    const std::int32_t yBound{shape.m_model->getHeight()};

    const std::int32_t r{geometrize::commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            shape.m_x1 = geometrize::commonutil::clamp(shape.m_x1 + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            shape.m_y1 = geometrize::commonutil::clamp(shape.m_y1 + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
        case 1:
        {
            shape.m_x2 = geometrize::commonutil::clamp(shape.m_x2 + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            shape.m_y2 = geometrize::commonutil::clamp(shape.m_y2 + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
        case 2:
        {
            shape.m_x3 = geometrize::commonutil::clamp(shape.m_x3 + geometrize::commonutil::randomRange(-32, 32), 0, xBound);
            shape.m_y3 = geometrize::commonutil::clamp(shape.m_y3 + geometrize::commonutil::randomRange(-32, 32), 0, yBound);
            break;
        }
    }
}

ShapeMutator::ShapeMutator()
{
    setDefaults();
}

void ShapeMutator::setDefaults()
{
    m_setupCircle = setupCircle;
    m_setupEllipse = setupEllipse;
    m_setupLine = setupLine;
    m_setupPolyline = setupPolyline;
    m_setupQuadraticBezier = setupQuadraticBezier;
    m_setupRectangle = setupRectangle;
    m_setupRotatedEllipse = setupRotatedEllipse;
    m_setupRotatedRectangle = setupRotatedRectangle;
    m_setupTriangle = setupTriangle;

    m_mutateCircle = mutateCircle;
    m_mutateEllipse = mutateEllipse;
    m_mutateLine = mutateLine;
    m_mutatePolyline = mutatePolyline;
    m_mutateQuadraticBezier = mutateQuadraticBezier;
    m_mutateRectangle = mutateRectangle;
    m_mutateRotatedEllipse = mutateRotatedEllipse;
    m_mutateRotatedRectangle = mutateRotatedRectangle;
    m_mutateTriangle = mutateTriangle;
}

void ShapeMutator::setup(geometrize::Circle& shape) const
{
    m_setupCircle(shape);
}

void ShapeMutator::mutate(geometrize::Circle& shape) const
{
    m_mutateCircle(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::Circle&)>& f)
{
    m_setupCircle = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::Circle&)>& f)
{
    m_mutateCircle = f;
}

void ShapeMutator::setup(geometrize::Ellipse& shape) const
{
    m_setupEllipse(shape);
}

void ShapeMutator::mutate(geometrize::Ellipse& shape) const
{
    m_mutateEllipse(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::Ellipse&)>& f)
{
    m_setupEllipse = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::Ellipse&)>& f)
{
    m_mutateEllipse = f;
}

void ShapeMutator::setup(geometrize::Line& shape) const
{
    m_setupLine(shape);
}

void ShapeMutator::mutate(geometrize::Line& shape) const
{
    m_mutateLine(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::Line&)>& f)
{
    m_setupLine = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::Line&)>& f)
{
    m_mutateLine = f;
}

void ShapeMutator::setup(geometrize::Polyline& shape) const
{
    m_setupPolyline(shape);
}

void ShapeMutator::mutate(geometrize::Polyline& shape) const
{
    m_mutatePolyline(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::Polyline&)>& f)
{
    m_setupPolyline = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::Polyline&)>& f)
{
    m_mutatePolyline = f;
}

void ShapeMutator::setup(geometrize::QuadraticBezier& shape) const
{
    m_setupQuadraticBezier(shape);
}

void ShapeMutator::mutate(geometrize::QuadraticBezier& shape) const
{
    m_mutateQuadraticBezier(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::QuadraticBezier&)>& f)
{
    m_setupQuadraticBezier = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::QuadraticBezier&)>& f)
{
    m_mutateQuadraticBezier = f;
}

void ShapeMutator::setup(geometrize::Rectangle& shape) const
{
    m_setupRectangle(shape);
}

void ShapeMutator::mutate(geometrize::Rectangle& shape) const
{
    m_mutateRectangle(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::Rectangle&)>& f)
{
    m_setupRectangle = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::Rectangle&)>& f)
{
    m_mutateRectangle = f;
}

void ShapeMutator::setup(geometrize::RotatedEllipse& shape) const
{
    m_setupRotatedEllipse(shape);
}

void ShapeMutator::mutate(geometrize::RotatedEllipse& shape) const
{
    m_mutateRotatedEllipse(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::RotatedEllipse&)>& f)
{
    m_setupRotatedEllipse = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::RotatedEllipse&)>& f)
{
    m_mutateRotatedEllipse = f;
}

void ShapeMutator::setup(geometrize::RotatedRectangle& shape) const
{
    m_setupRotatedRectangle(shape);
}

void ShapeMutator::mutate(geometrize::RotatedRectangle& shape) const
{
    m_mutateRotatedRectangle(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::RotatedRectangle&)>& f)
{
    m_setupRotatedRectangle = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::RotatedRectangle&)>& f)
{
    m_mutateRotatedRectangle = f;
}

void ShapeMutator::setup(geometrize::Triangle& shape) const
{
    m_setupTriangle(shape);
}

void ShapeMutator::mutate(geometrize::Triangle& shape) const
{
    m_mutateTriangle(shape);
}

void ShapeMutator::setSetupFunction(const std::function<void(geometrize::Triangle&)>& f)
{
    m_setupTriangle = f;
}

void ShapeMutator::setMutatorFunction(const std::function<void(geometrize::Triangle&)>& f)
{
    m_mutateTriangle = f;
}

}
