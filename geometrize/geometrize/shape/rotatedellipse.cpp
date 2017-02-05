#include "rotatedellipse.h"

#include <cstdint>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

RotatedEllipse::RotatedEllipse(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    // TODO
}

geometrize::Shape* RotatedEllipse::clone() const
{
    return new geometrize::RotatedEllipse(m_xBound, m_yBound); // TODO
}

std::vector<Scanline> RotatedEllipse::rasterize() const
{
    std::vector<geometrize::Scanline> lines;
    return lines; // TODO
}

void RotatedEllipse::mutate()
{
    // TODO
}

geometrize::shapes::ShapeTypes RotatedEllipse::getType() const
{
    return geometrize::shapes::ShapeTypes::ROTATED_ELLIPSE;
}

std::vector<std::int32_t> RotatedEllipse::getShapeData() const
{
    return {};
}

}
