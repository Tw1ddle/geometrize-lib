#include "triangle.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Triangle::Triangle(const std::int32_t xBound, const std::int32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    m_x1 = commonutil::randomRange(0, xBound);
    m_y1 = commonutil::randomRange(0, yBound);
    m_x2 = m_x1 + commonutil::randomRange(-16, 16);
    m_y2 = m_y1 + commonutil::randomRange(-16, 16);
    m_x3 = m_x1 + commonutil::randomRange(-16, 16);
    m_y3 = m_y1 + commonutil::randomRange(-16, 16);
}

std::shared_ptr<geometrize::Shape> Triangle::clone() const
{
    std::shared_ptr<geometrize::Triangle> triangle{std::make_shared<geometrize::Triangle>(m_xBound, m_yBound)};
    triangle->m_x1 = m_x1;
    triangle->m_y1 = m_y1;
    triangle->m_x2 = m_x2;
    triangle->m_y2 = m_y2;
    triangle->m_x3 = m_x3;
    triangle->m_y3 = m_y3;
    return triangle;
}

std::vector<geometrize::Scanline> Triangle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;

    // TODO

    return lines;
}

void Triangle::mutate()
{
    const std::int32_t r{commonutil::randomRange(0, 2)};

    switch(r) {
    case 0:
    {
        break;
    }
    case 1:
    {
        break;
    }
    case 2:
    {
        break;
    }
    }
}

geometrize::shapes::ShapeTypes Triangle::getType() const
{
    return shapes::ShapeTypes::TRIANGLE;
}

std::vector<std::int32_t> Triangle::getRawShapeData() const
{
    return { m_x1, m_y1, m_x2, m_y2, m_x3, m_y3 };
}

std::string Triangle::getSvgShapeData() const
{
    return "TODO";
}

}
