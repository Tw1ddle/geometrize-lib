#include "rectangle.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"

namespace geometrize
{

Rectangle::Rectangle(const float x1, const float y1, const float x2, const float y2) : Shape()
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
}

std::shared_ptr<geometrize::Shape> Rectangle::clone() const
{
    std::shared_ptr<geometrize::Rectangle> rect{std::make_shared<geometrize::Rectangle>()};
    rect->m_x1 = m_x1;
    rect->m_y1 = m_y1;
    rect->m_x2 = m_x2;
    rect->m_y2 = m_y2;
    rect->setup = setup;
    rect->mutate = mutate;
    rect->rasterize = rasterize;
    return rect;
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
        ((std::fmin)(m_x1, m_x2)),
        ((std::fmin)(m_y1, m_y2)),
        ((std::fmax)(m_x1, m_x2)),
        ((std::fmax)(m_y1, m_y2))
    };
}

std::string Rectangle::getSvgShapeData() const
{
    std::stringstream s;
    s << "<rect x=\"" << (std::fmin)(m_x1, m_x2) << "\" y=\"" << (std::fmin)(m_y1, m_y2) << "\" width=\"" << (std::fmax)(m_x1, m_x2) - (std::fmin)(m_x1, m_x2) << "\" height=\"" << (std::fmax)(m_y1, m_y2) - (std::fmin)(m_y1, m_y2) << "\" " << SVG_STYLE_HOOK << " />";
    return s.str();
}

}
