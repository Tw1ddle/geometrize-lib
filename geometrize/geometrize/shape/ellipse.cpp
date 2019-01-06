#include "ellipse.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Ellipse::Ellipse(const geometrize::Model& model) : Shape{model}
{
    m_model->setupShape(*this);
}

Ellipse::Ellipse(const float x, const float y, const float rx, const float ry) : Shape()
{
    m_x = x;
    m_y = y;
    m_rx = rx;
    m_ry = ry;
}

std::shared_ptr<geometrize::Shape> Ellipse::clone() const
{
    std::shared_ptr<geometrize::Ellipse> ellipse{std::make_shared<geometrize::Ellipse>(*m_model)};
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

    const std::int32_t w{m_model->getWidth()};
    const std::int32_t h{m_model->getHeight()};

    for (std::int32_t dy = 0; dy < m_ry; dy++) {
        const std::int32_t y1{static_cast<std::int32_t>(m_y) - dy};
        const std::int32_t y2{static_cast<std::int32_t>(m_y) + dy};

        if ((y1 < 0 || y1 >= h) && (y2 < 0 || y2 >= h)) {
            continue;
        }

        const std::int32_t s{static_cast<std::int32_t>(std::sqrt(m_ry * m_ry - dy * dy) * aspect)};
        std::int32_t x1{static_cast<std::int32_t>(m_x) - s};
        std::int32_t x2{static_cast<std::int32_t>(m_x) + s};
        if (x1 < 0) {
            x1 = 0;
        }
        if (x2 >= w) {
            x2 = w - 1;
        }

        if (y1 >= 0 && y1 < h) {
            lines.push_back(Scanline(y1, x1, x2));
        }
        if (y2 >= 0 && y2 < h && dy > 0) {
            lines.push_back(Scanline(y2, x1, x2));
        }
    }

    return geometrize::Scanline::trim(lines, m_model->getWidth(), m_model->getHeight());
}

void Ellipse::mutate()
{
    m_model->mutateShape(*this);
}

void Ellipse::translate(const float x, const float y)
{
    m_x += x;
    m_y += y;
}

void Ellipse::scale(const float scaleFactor)
{
    m_rx *= scaleFactor;
    m_ry *= scaleFactor;
}

geometrize::ShapeTypes Ellipse::getType() const
{
    return geometrize::ShapeTypes::ELLIPSE;
}

std::vector<float> Ellipse::getRawShapeData() const
{
    return { m_x, m_y, m_rx, m_ry };
}

std::string Ellipse::getSvgShapeData() const
{
    std::stringstream s;
    s << "<ellipse cx=\"" << m_x << "\" cy=\"" << m_y << "\" rx=\"" << m_rx << "\" ry=\"" << m_ry << "\" " << SVG_STYLE_HOOK << " />";
    return s.str();
}

}
