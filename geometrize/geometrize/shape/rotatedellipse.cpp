#include "rotatedellipse.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

RotatedEllipse::RotatedEllipse(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    // TODO
}

std::shared_ptr<geometrize::Shape> RotatedEllipse::clone() const
{
    return std::make_shared<geometrize::RotatedEllipse>(m_xBound, m_yBound); // TODO
}

std::vector<geometrize::Scanline> RotatedEllipse::rasterize() const
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
