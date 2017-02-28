#include "spline.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Spline::Spline(const std::int32_t xBound, const std::int32_t yBound) : m_xBound(xBound), m_yBound(yBound), m_interpolationMode{SplineInterpolationMode::CARDINAL}
{
    // TODO create initial control points
}

std::shared_ptr<geometrize::Shape> Spline::clone() const
{
    std::shared_ptr<geometrize::Spline> spline{std::make_shared<geometrize::Spline>(m_xBound, m_yBound)};
    spline->m_interpolationMode = m_interpolationMode;
    spline->m_controlPoints = m_controlPoints;
    return spline;
}

std::vector<geometrize::Scanline> Spline::rasterize() const
{
    std::vector<geometrize::Scanline> lines;

    // TODO

    return lines;
}

void Spline::mutate()
{
    // TODO

    const std::int32_t r{commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            break;
        }
        case 1:
        {
            break;
        }
    }
}

geometrize::shapes::ShapeTypes Spline::getType() const
{
    return geometrize::shapes::ShapeTypes::SPLINE;
}

std::vector<std::int32_t> Spline::getShapeData() const
{
    return {};
}

}
