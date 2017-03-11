#include "rectangle.h"

#include <algorithm>
#include <cstdint>
#include <memory>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Rectangle::Rectangle(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    m_x1 = commonutil::randomRange(0, xBound - 1);
    m_y1 = commonutil::randomRange(0, yBound - 1);
    m_x2 = commonutil::clamp(m_x1 + commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1), 0, xBound - 1);
    m_y2 = commonutil::clamp(m_y1 + commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1), 0, yBound - 1);
}

std::shared_ptr<geometrize::Shape> Rectangle::clone() const
{
    std::shared_ptr<geometrize::Rectangle> rect{std::make_shared<geometrize::Rectangle>(m_model)};
    rect->m_x1 = m_x1;
    rect->m_y1 = m_y1;
    rect->m_x2 = m_x2;
    rect->m_y2 = m_y2;
    return rect;
}

std::vector<geometrize::Scanline> Rectangle::rasterize() const
{
    const std::int32_t y1{(std::min)(m_y1, m_y2)};
    const std::int32_t y2{(std::max)(m_y1, m_y2)};

    std::vector<geometrize::Scanline> lines;
    for(std::int32_t y = y1; y < y2; y++) {
        lines.push_back(geometrize::Scanline(y, (std::min)(m_x1, m_x2), (std::max)(m_x1, m_x2), 0xFFFF));
    }
    return lines;
}

void Rectangle::mutate()
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

geometrize::shapes::ShapeTypes Rectangle::getType() const
{
    return geometrize::shapes::ShapeTypes::RECTANGLE;
}

std::vector<std::int32_t> Rectangle::getRawShapeData() const
{
    return {
        ((std::min)(m_x1, m_x2)),
        ((std::min)(m_y1, m_y2)),
        ((std::max)(m_x1, m_x2)),
        ((std::max)(m_y1, m_y2))
    };
}

std::string Rectangle::getSvgShapeData(const std::string& attribs) const
{
    return "TODO";
}

}
