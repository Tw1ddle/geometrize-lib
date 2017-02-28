#include "circle.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Circle::Circle(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    m_x = commonutil::randomRange(0, m_xBound);
    m_y = commonutil::randomRange(0, m_yBound);
    m_r = commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1);
}

std::shared_ptr<geometrize::Shape> Circle::clone() const
{
    std::shared_ptr<geometrize::Circle> circle{std::make_shared<geometrize::Circle>(m_xBound, m_yBound)};
    circle->m_x = m_x;
    circle->m_y = m_y;
    circle->m_r = m_r;
    return circle;
}

std::vector<geometrize::Scanline> Circle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;

    const std::uint32_t w{m_xBound};
    const std::uint32_t h{m_yBound};

    const std::int32_t r{static_cast<std::int32_t>(m_r)};
    for(std::int32_t y = -r; y <= r; y++) {
        std::vector<std::int32_t> xScan;
        for(std::int32_t x = -r; x <= r; x++) {
            if(x * x + y * y <= r * r) {
                xScan.push_back(x);
            }
        }

        if(!xScan.empty()) {
            const std::uint32_t fy{commonutil::clamp(m_y + y, 0U, h - 1)};
            const std::uint32_t x1{commonutil::clamp(m_x + xScan.front(), 0U, w - 1)};
            const std::uint32_t x2{commonutil::clamp(m_x + xScan.back(), 0U, w - 1)};
            lines.push_back(geometrize::Scanline(fy, x1, x2, 0xFFFF));
        }
    }

    return lines;
}

void Circle::mutate()
{
    const std::int32_t r{commonutil::randomRange(0, 1)};
    switch(r) {
        case 0:
        {
            m_y = commonutil::clamp(m_x + commonutil::randomRange(-16, 16), 0U, m_xBound);
            m_y = commonutil::clamp(m_y + commonutil::randomRange(-16, 16), 0U, m_yBound);
            break;
        }
        case 1:
        {
            m_r = commonutil::clamp(m_r + commonutil::randomRange(-16, 16), 1U, m_xBound);
            break;
        }
    }
}

geometrize::shapes::ShapeTypes Circle::getType() const
{
    return geometrize::shapes::ShapeTypes::CIRCLE;
}

std::vector<std::int32_t> Circle::getShapeData() const
{
    return { static_cast<std::int32_t>(m_x), static_cast<std::int32_t>(m_y), static_cast<std::int32_t>(m_r) };
}

}
