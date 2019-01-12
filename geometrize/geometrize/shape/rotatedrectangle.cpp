#include "rotatedrectangle.h"

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

RotatedRectangle::RotatedRectangle(const float x1, const float y1, const float x2, const float y2, const float angle) : Shape()
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
    m_angle = angle;
}

std::shared_ptr<geometrize::Shape> RotatedRectangle::clone() const
{
    std::shared_ptr<geometrize::RotatedRectangle> rect{std::make_shared<geometrize::RotatedRectangle>()};
    rect->m_x1 = m_x1;
    rect->m_y1 = m_y1;
    rect->m_x2 = m_x2;
    rect->m_y2 = m_y2;
    rect->m_angle = m_angle;
    rect->setup = setup;
    rect->mutate = mutate;
    rect->rasterize = rasterize;
    return rect;
}

geometrize::ShapeTypes RotatedRectangle::getType() const
{
    return geometrize::ShapeTypes::ROTATED_RECTANGLE;
}

}
