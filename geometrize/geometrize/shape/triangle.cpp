#include "triangle.h"

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <utility>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Triangle::Triangle(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    m_x1 = commonutil::randomRange(0, xBound - 1);
    m_y1 = commonutil::randomRange(0, yBound - 1);
    m_x2 = m_x1 + commonutil::randomRange(-16, 16);
    m_y2 = m_y1 + commonutil::randomRange(-16, 16);
    m_x3 = m_x1 + commonutil::randomRange(-16, 16);
    m_y3 = m_y1 + commonutil::randomRange(-16, 16);
}

std::shared_ptr<geometrize::Shape> Triangle::clone() const
{
    std::shared_ptr<geometrize::Triangle> triangle{std::make_shared<geometrize::Triangle>(m_model)};
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

    const std::vector<std::pair<std::int32_t, std::int32_t>> p1p2{commonutil::bresenham(m_x1, m_y1, m_x2, m_y2)};
    const std::vector<std::pair<std::int32_t, std::int32_t>> p2p3{commonutil::bresenham(m_x2, m_y2, m_x3, m_y3)};
    const std::vector<std::pair<std::int32_t, std::int32_t>> p3p1{commonutil::bresenham(m_x3, m_y3, m_x1, m_y1)};

    std::vector<std::pair<std::int32_t, std::int32_t>> edges;
    edges.insert(edges.end(), p1p2.begin(), p1p2.end());
    edges.insert(edges.end(), p2p3.begin(), p2p3.end());
    edges.insert(edges.end(), p3p1.begin(), p3p1.end());

    std::map<std::int32_t, std::set<std::int32_t>> yToXs;
    for(std::pair<std::int32_t, std::int32_t> point : edges) {
        yToXs[point.second].insert(point.first);
    }

    for(const auto& it : yToXs) {
        geometrize::Scanline scanline(it.first,
                                    *(std::min_element(it.second.begin(), it.second.end())),
                                    *(std::max_element(it.second.begin(), it.second.end())),
                                    0xFFFF);
        lines.push_back(scanline);
    }

    return Scanline::trim(lines, m_model.getWidth(), m_model.getHeight());
}

void Triangle::mutate()
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::int32_t r{commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            m_x1 = commonutil::clamp(m_x1 + commonutil::randomRange(-16, 16), 0, xBound);
            m_y1 = commonutil::clamp(m_y1 + commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 1:
        {
            m_x2 = commonutil::clamp(m_x2 + commonutil::randomRange(-16, 16), 0, xBound);
            m_y2 = commonutil::clamp(m_y2 + commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 2:
        {
            m_x3 = commonutil::clamp(m_x3 + commonutil::randomRange(-16, 16), 0, xBound);
            m_y3 = commonutil::clamp(m_y3 + commonutil::randomRange(-16, 16), 0, yBound);
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
    std::stringstream s;
    s << "<polygon "
      << "points=\""
      << m_x1 << "," << m_y1 << " "
      << m_x2 << "," << m_y2 << " "
      << m_x3 << "," << m_y3
      << SVG_STYLE_HOOK << " "
      << "/>";

    return s.str();
}

}
