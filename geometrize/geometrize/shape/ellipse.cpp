#include "ellipse.h"

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

Ellipse::Ellipse(const float x, const float y, const float rx, const float ry) : Shape()
{
    m_x = x;
    m_y = y;
    m_rx = rx;
    m_ry = ry;
}

std::shared_ptr<geometrize::Shape> Ellipse::clone() const
{
    std::shared_ptr<geometrize::Ellipse> ellipse{std::make_shared<geometrize::Ellipse>()};
    ellipse->m_x = m_x;
    ellipse->m_y = m_y;
    ellipse->m_rx = m_rx;
    ellipse->m_ry = m_ry;
    ellipse->setup = setup;
    ellipse->mutate = mutate;
    ellipse->rasterize = rasterize;
    return ellipse;
}

geometrize::ShapeTypes Ellipse::getType() const
{
    return geometrize::ShapeTypes::ELLIPSE;
}

}
