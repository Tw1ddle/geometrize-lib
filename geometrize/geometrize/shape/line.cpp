#include "line.h"

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

Line::Line(const float x1, const float y1, const float x2, const float y2) : Shape()
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
}

std::shared_ptr<geometrize::Shape> Line::clone() const
{
    std::shared_ptr<geometrize::Line> line{std::make_shared<geometrize::Line>()};
    line->m_x1 = m_x1;
    line->m_y1 = m_y1;
    line->m_x2 = m_x2;
    line->m_y2 = m_y2;
    line->setup = setup;
    line->mutate = mutate;
    line->rasterize = rasterize;
    return line;
}

geometrize::ShapeTypes Line::getType() const
{
    return geometrize::ShapeTypes::LINE;
}

}
