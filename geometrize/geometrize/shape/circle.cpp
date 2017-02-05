#include "circle.h"

#include <cstdint>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Circle::Circle(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    // TODO
}

geometrize::Shape* Circle::clone() const
{
    return new Circle(m_xBound, m_yBound); // TODO
}

std::vector<geometrize::Scanline> Circle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;
    return lines; // TODO
}

void Circle::mutate()
{
    // TODO
}

geometrize::shapes::ShapeTypes Circle::getType() const
{
    return geometrize::shapes::ShapeTypes::CIRCLE;
}

std::vector<std::int32_t> Circle::getShapeData() const
{
    return {};
}

}
