#include "rectangle.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Rectangle::Rectangle(const geometrize::Model& model) : Shape{model}
{
    m_model.setupShape(*this);
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
    const std::int32_t x1{(std::min)(m_x1, m_x2)};
    const std::int32_t x2{(std::max)(m_x1, m_x2)};
    const std::int32_t y1{(std::min)(m_y1, m_y2)};
    const std::int32_t y2{(std::max)(m_y1, m_y2)};

    std::vector<geometrize::Scanline> lines;
    for(std::int32_t y = y1; y < y2; y++) {
        lines.push_back(geometrize::Scanline(y, x1, x2));
    }
    return geometrize::Scanline::trim(lines, m_model.getWidth(), m_model.getHeight());
}

void Rectangle::mutate()
{
    m_model.mutateShape(*this);
}

geometrize::ShapeTypes Rectangle::getType() const
{
    return geometrize::ShapeTypes::RECTANGLE;
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

std::string Rectangle::getSvgShapeData() const
{
    std::stringstream s;
    s << "<rect x=\"" << (std::min)(m_x1, m_x2) << "\" y=\"" << (std::min)(m_y1, m_y2) << "\" width=\"" << (std::max)(m_x1, m_x2) - (std::min)(m_x1, m_x2) << "\" height=\"" << (std::max)(m_y1, m_y2) - (std::min)(m_y1, m_y2) << "\" " << SVG_STYLE_HOOK << " />";
    return s.str();
}

}
