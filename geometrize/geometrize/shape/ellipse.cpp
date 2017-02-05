#include "ellipse.h"

#include <cstdint>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Ellipse::Ellipse(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    // TODO
}

geometrize::Shape* Ellipse::clone() const
{
    return new Ellipse(m_xBound, m_yBound); // TODO
}

std::vector<geometrize::Scanline> Ellipse::rasterize() const
{
    std::vector<geometrize::Scanline> lines;
    return lines; // TODO
}

void Ellipse::mutate()
{
    // TODO
}

geometrize::shapes::ShapeTypes Ellipse::getType() const
{
    return geometrize::shapes::ShapeTypes::ELLIPSE;
}

std::vector<std::int32_t> Ellipse::getShapeData() const
{
    return {};
}

}
