#include "line.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Line::Line(const geometrize::Model& model, const std::int32_t xBound, const std::int32_t yBound) : m_model{model}, m_xBound(xBound), m_yBound(yBound)
{
    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(commonutil::randomRange(0, m_xBound), commonutil::randomRange(0, m_yBound - 1))};

    m_x1 = commonutil::clamp(startingPoint.first + commonutil::randomRange(-32, 32), 0, m_xBound - 1);
    m_y1 = commonutil::clamp(startingPoint.second + commonutil::randomRange(-32, 32), 0, m_yBound - 1);
    m_x2 = commonutil::clamp(startingPoint.first + commonutil::randomRange(-32, 32), 0, m_xBound - 1);
    m_y2 = commonutil::clamp(startingPoint.second + commonutil::randomRange(-32, 32), 0, m_yBound - 1);
}

std::shared_ptr<geometrize::Shape> Line::clone() const
{
    std::shared_ptr<geometrize::Line> line{std::make_shared<geometrize::Line>(m_model, m_xBound, m_yBound)};
    line->m_x1 = m_x1;
    line->m_y1 = m_y1;
    line->m_x2 = m_x2;
    line->m_y2 = m_y2;
    return line;
}

std::vector<geometrize::Scanline> Line::rasterize() const
{
    std::vector<geometrize::Scanline> lines;

    const auto points{commonutil::bresenham(m_x1, m_y1, m_x2, m_y2)};
    for(const auto& point : points) {
       lines.push_back(geometrize::Scanline(point.second, point.first, point.first, 0xFFFF));
    }

    return Scanline::trim(lines, m_xBound, m_yBound);
}

void Line::mutate()
{
    const std::int32_t r{commonutil::randomRange(0, 1)};

    switch(r) {
        case 0:
        {
            m_x1 = commonutil::clamp(m_x1 + commonutil::randomRange(-16, 16), 0, m_xBound - 1);
            m_y1 = commonutil::clamp(m_y1 + commonutil::randomRange(-16, 16), 0, m_yBound - 1);
            break;
        }
        case 1:
        {
            m_x2 = commonutil::clamp(m_x2 + commonutil::randomRange(-16, 16), 0, m_xBound - 1);
            m_y2 = commonutil::clamp(m_y2 + commonutil::randomRange(-16, 16), 0, m_yBound - 1);
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
    return "TODO";
}

}
