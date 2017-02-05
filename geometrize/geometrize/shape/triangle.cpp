#include "triangle.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Triangle::Triangle(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    // TODO
}

std::shared_ptr<geometrize::Shape> Triangle::clone() const
{
    return std::make_shared<geometrize::Triangle>(m_xBound, m_yBound); // TODO
}

std::vector<geometrize::Scanline> Triangle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;
    return lines; // TODO
}

void Triangle::mutate()
{
    // TODO
}

geometrize::shapes::ShapeTypes Triangle::getType() const
{
    return shapes::ShapeTypes::TRIANGLE;
}

std::vector<std::int32_t> Triangle::getShapeData() const
{
    return {};
}

}
