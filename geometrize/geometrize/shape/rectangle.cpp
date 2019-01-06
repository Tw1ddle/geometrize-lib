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
    m_model->setupShape(*this);
}

Rectangle::Rectangle(const float x1, const float y1, const float x2, const float y2) : Shape()
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
}

std::shared_ptr<geometrize::Shape> Rectangle::clone() const
{
    std::shared_ptr<geometrize::Rectangle> rect{std::make_shared<geometrize::Rectangle>(*m_model)};
    rect->m_x1 = m_x1;
    rect->m_y1 = m_y1;
    rect->m_x2 = m_x2;
    rect->m_y2 = m_y2;
    return rect;
}

std::vector<geometrize::Scanline> Rectangle::rasterize() const
{
    const std::int32_t x1{static_cast<std::int32_t>((std::min)(m_x1, m_x2))};
    const std::int32_t x2{static_cast<std::int32_t>((std::max)(m_x1, m_x2))};
    const std::int32_t y1{static_cast<std::int32_t>((std::min)(m_y1, m_y2))};
    const std::int32_t y2{static_cast<std::int32_t>((std::max)(m_y1, m_y2))};

    std::vector<geometrize::Scanline> lines;
    for(std::int32_t y = y1; y < y2; y++) {
        lines.push_back(geometrize::Scanline(y, x1, x2));
    }
    return geometrize::Scanline::trim(lines, m_model->getWidth(), m_model->getHeight());
}

void Rectangle::mutate()
{
    m_model->mutateShape(*this);
}

void Rectangle::translate(const float x, const float y)
{
    m_x1 += x;
    m_y1 += y;
    m_x2 += x;
    m_y2 += y;
}

void Rectangle::scale(const float scaleFactor)
{
    // TODO
}

geometrize::ShapeTypes Rectangle::getType() const
{
    return geometrize::ShapeTypes::RECTANGLE;
}

std::vector<float> Rectangle::getRawShapeData() const
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
