#include "rectangle.h"

#include <cstdint>
#include <memory>

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

geometrize::ShapeTypes Rectangle::getType() const
{
    return geometrize::ShapeTypes::RECTANGLE;
}

}
