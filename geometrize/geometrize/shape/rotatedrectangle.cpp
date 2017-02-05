#include "rotatedrectangle.h"

#include <cstdint>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

RotatedRectangle::RotatedRectangle(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    // TODO
}

geometrize::Shape* RotatedRectangle::clone() const
{
    return new geometrize::RotatedRectangle(m_xBound, m_yBound); // TODO
}

std::vector<geometrize::Scanline> RotatedRectangle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;
    return lines; // TODO
}

void RotatedRectangle::mutate()
{
    // TODO
}

geometrize::shapes::ShapeTypes RotatedRectangle::getType() const
{
    return geometrize::shapes::ShapeTypes::ROTATED_RECTANGLE;
}

std::vector<std::int32_t> RotatedRectangle::getShapeData() const
{
    return {};
}

}
