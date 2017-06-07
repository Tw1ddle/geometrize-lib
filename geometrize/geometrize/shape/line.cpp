#include "line.h"

#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../commonutil.h"
#include "../model.h"
#include "shapeutil.h"

namespace geometrize
{

Line::Line(const geometrize::Model& model) : Shape{model}
{
    m_model.setupShape(*this);
}

std::shared_ptr<geometrize::Shape> Line::clone() const
{
    std::shared_ptr<geometrize::Line> line{std::make_shared<geometrize::Line>(m_model)};
    line->m_x1 = m_x1;
    line->m_y1 = m_y1;
    line->m_x2 = m_x2;
    line->m_y2 = m_y2;
    return line;
}

std::vector<geometrize::Scanline> Line::rasterize() const
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    std::vector<geometrize::Scanline> lines;

    const auto points{geometrize::bresenham(m_x1, m_y1, m_x2, m_y2)};
    for(const auto& point : points) {
       lines.push_back(geometrize::Scanline(point.second, point.first, point.first));
    }

    return Scanline::trim(lines, xBound, yBound);
}

void Line::mutate()
{
    m_model.mutateShape(*this);
}

geometrize::ShapeTypes Line::getType() const
{
    return geometrize::ShapeTypes::LINE;
}

std::vector<std::int32_t> Line::getRawShapeData() const
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
