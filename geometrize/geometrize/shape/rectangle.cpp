#include "rectangle.h"

#include <algorithm>
#include <cstdint>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Rectangle::Rectangle(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    m_x1 = commonutil::randomRange(0, m_xBound);
    m_y1 = commonutil::randomRange(0, m_yBound);
    m_x2 = commonutil::clamp(m_x1 + commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1), 0U, m_xBound);
    m_y2 = commonutil::clamp(m_y1 + commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1), 0U, m_yBound);
}

geometrize::Shape* Rectangle::clone() const
{
    geometrize::Rectangle* rect{new Rectangle(m_xBound, m_yBound)};
    rect->m_x1 = m_x1;
    rect->m_y1 = m_y1;
    rect->m_x2 = m_x2;
    rect->m_y2 = m_y2;
    return rect;
}

std::vector<Scanline> Rectangle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;
    for(std::uint32_t y = m_y1; y < m_y2; y++) {
        if(m_x1 != m_x2) {
            lines.push_back(geometrize::Scanline(y, (std::min)(m_x1, m_x2), (std::max)(m_x1, m_x2), 0xFFFF));
        }
    }
    return lines;
}

void Rectangle::mutate()
{
    const int r = commonutil::randomRange(0, 1);

    switch(r) {
    case 0:
        m_x1 = commonutil::clamp(m_x1 + commonutil::randomRange(-16, 16), 0U, m_xBound - 1);
        m_y1 = commonutil::clamp(m_y1 + commonutil::randomRange(-16, 16), 0U, m_yBound - 1);
    case 1:
        m_x2 = commonutil::clamp(m_x2 + commonutil::randomRange(-16, 16), 0U, m_xBound - 1);
        m_y2 = commonutil::clamp(m_y2 + commonutil::randomRange(-16, 16), 0U, m_yBound - 1);
    }
}

geometrize::shapes::ShapeTypes Rectangle::getType() const
{
    return geometrize::shapes::ShapeTypes::RECTANGLE;
}

std::vector<std::int32_t> Rectangle::getShapeData() const
{
    return {
        static_cast<std::int32_t>(m_x1),
        static_cast<std::int32_t>(m_y1),
        static_cast<std::int32_t>(m_x2),
        static_cast<std::int32_t>(m_y2)
    };
}

}
