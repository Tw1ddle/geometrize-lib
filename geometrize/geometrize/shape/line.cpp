#include "line.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

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

void Line::translate(const float x, const float y)
{
    m_x1 += x;
    m_y1 += y;
    m_x2 += x;
    m_y2 += y;
}

void Line::scale(const float scaleFactor)
{
    // TODO
}

geometrize::ShapeTypes Line::getType() const
{
    return geometrize::ShapeTypes::LINE;
}

std::vector<float> Line::getRawShapeData() const
{
    return { m_x1, m_y1, m_x2, m_y2 };
}

std::string Line::getSvgShapeData() const
{
    std::stringstream s;
    s << "<line x1=\"" << m_x1 << "\" y1=\"" << m_y1 << "\" x2=\"" << m_x2 << "\" y2=\"" << m_y2 << "\" " << SVG_STYLE_HOOK << " />";
    return s.str();
}

}
