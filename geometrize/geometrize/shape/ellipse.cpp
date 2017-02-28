#include "ellipse.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Ellipse::Ellipse(const std::int32_t xBound, const std::int32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    m_x = commonutil::randomRange(0, m_xBound);
    m_y = commonutil::randomRange(0, m_yBound);
    m_rx = commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1);
    m_ry = commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1);
}

std::shared_ptr<geometrize::Shape> Ellipse::clone() const
{
    std::shared_ptr<geometrize::Ellipse> ellipse{std::make_shared<geometrize::Ellipse>(m_xBound, m_yBound)};
    ellipse->m_x = m_x;
    ellipse->m_y = m_y;
    ellipse->m_rx = m_rx;
    ellipse->m_ry = m_ry;
    return ellipse;
}

std::vector<geometrize::Scanline> Ellipse::rasterize() const
{
    std::vector<geometrize::Scanline> lines;

    const float aspect{static_cast<float>(m_rx) / static_cast<float>(m_ry)};

    const std::int32_t w{m_xBound};
    const std::int32_t h{m_yBound};

    for (std::int32_t dy = 0; dy < m_ry; dy++) {
        const std::int32_t y1{m_y - dy};
        const std::int32_t y2{m_y + dy};

        if ((y1 < 0 || y1 >= h) && (y2 < 0 || y2 >= h)) {
            continue;
        }

        const std::int32_t s{static_cast<std::int32_t>(std::sqrt(m_ry * m_ry - dy * dy) * aspect)};
        std::int32_t x1{m_x - s};
        std::int32_t x2{m_x + s};
        if (x1 < 0) {
            x1 = 0;
        }
        if (x2 >= w) {
            x2 = w - 1;
        }

        if (y1 >= 0 && y1 < h) {
            lines.push_back(Scanline(y1, x1, x2, 0xFFFF));
        }
        if (y2 >= 0 && y2 < h && dy > 0) {
            lines.push_back(Scanline(y2, x1, x2, 0xFFFF));
        }
    }

    return lines;
}

void Ellipse::mutate()
{
    const std::int32_t r{commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            m_y = commonutil::clamp(m_x + commonutil::randomRange(-16, 16), 0, m_xBound);
            m_y = commonutil::clamp(m_y + commonutil::randomRange(-16, 16), 0, m_yBound);
            break;
        }
        case 1:
        {
            m_rx = commonutil::clamp(m_rx + commonutil::randomRange(-16, 16), 1, m_xBound);
            break;
        }
        case 2:
        {
            m_ry = commonutil::clamp(m_ry + commonutil::randomRange(-16, 16), 1, m_xBound);
            break;
        }
    }
}

geometrize::shapes::ShapeTypes Ellipse::getType() const
{
    return geometrize::shapes::ShapeTypes::ELLIPSE;
}

std::vector<std::int32_t> Ellipse::getShapeData() const
{
    return { m_x, m_y, m_rx, m_ry };
}

}
