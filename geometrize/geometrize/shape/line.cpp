#include "line.h"

#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Line::Line(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(commonutil::randomRange(0, xBound), commonutil::randomRange(0, yBound - 1))};

    m_x1 = commonutil::clamp(startingPoint.first + commonutil::randomRange(-32, 32), 0, xBound - 1);
    m_y1 = commonutil::clamp(startingPoint.second + commonutil::randomRange(-32, 32), 0, yBound - 1);
    m_x2 = commonutil::clamp(startingPoint.first + commonutil::randomRange(-32, 32), 0, xBound - 1);
    m_y2 = commonutil::clamp(startingPoint.second + commonutil::randomRange(-32, 32), 0, yBound - 1);
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

    const auto points{commonutil::bresenham(m_x1, m_y1, m_x2, m_y2)};
    for(const auto& point : points) {
       lines.push_back(geometrize::Scanline(point.second, point.first, point.first, 0xFFFF));
    }

    return Scanline::trim(lines, xBound, yBound);
}

void Line::mutate()
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::int32_t r{commonutil::randomRange(0, 1)};

    switch(r) {
        case 0:
        {
            m_x1 = commonutil::clamp(m_x1 + commonutil::randomRange(-16, 16), 0, xBound - 1);
            m_y1 = commonutil::clamp(m_y1 + commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            m_x2 = commonutil::clamp(m_x2 + commonutil::randomRange(-16, 16), 0, xBound - 1);
            m_y2 = commonutil::clamp(m_y2 + commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
    }
}

geometrize::shapes::ShapeTypes Line::getType() const
{
    return geometrize::shapes::ShapeTypes::LINE;
}

std::vector<std::int32_t> Line::getRawShapeData() const
{
    return { m_x1, m_y1, m_x2, m_y2 };
}

std::string Line::getSvgShapeData() const
{
    std::stringstream s;
    s << "<line "
      << "x1=\"" << m_x1 << "\" "
      << "y1=\"" << m_y1 << "\" "
      << "x2=\"" << m_x2 << "\" "
      << "y2=\"" << m_y2 << "\" "
      << SVG_STYLE_HOOK << " "
      << "/>";

    return s.str();
}

}
