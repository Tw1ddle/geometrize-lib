#include "quadraticbezier.h"

#include <cstdint>
#include <memory>
#include "shape.h"

namespace geometrize
{

QuadraticBezier::QuadraticBezier(const float cx, const float cy, const float x1, const float y1, const float x2, const float y2) : Shape()
{
    m_cx = cx;
    m_cy = cy;
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
}

std::shared_ptr<geometrize::Shape> QuadraticBezier::clone() const
{
    std::shared_ptr<geometrize::QuadraticBezier> bezier{std::make_shared<geometrize::QuadraticBezier>()};
    bezier->m_x1 = m_x1;
    bezier->m_y1 = m_y1;
    bezier->m_cx = m_cx;
    bezier->m_cy = m_cy;
    bezier->m_x2 = m_x2;
    bezier->m_y2 = m_y2;
    bezier->setup = setup;
    bezier->mutate = mutate;
    bezier->rasterize = rasterize;
    return bezier;
}

geometrize::ShapeTypes QuadraticBezier::getType() const
{
    return geometrize::ShapeTypes::QUADRATIC_BEZIER;
}

}
