#include "quadraticbezier.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>

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

void QuadraticBezier::translate(const float x, const float y)
{
    m_cx += x;
    m_cy += y;
    m_x1 += x;
    m_y1 += y;
    m_x2 += x;
    m_y2 += y;
}

void QuadraticBezier::scale(const float scaleFactor)
{
    // TODO
}

geometrize::ShapeTypes QuadraticBezier::getType() const
{
    return geometrize::ShapeTypes::QUADRATIC_BEZIER;
}

std::vector<float> QuadraticBezier::getRawShapeData() const
{
    return { m_x1, m_y1, m_cx, m_cy, m_x2, m_y2 };
}

std::string QuadraticBezier::getSvgShapeData() const
{
    std::stringstream s;
    s << "<path d=\"M" << m_x1 << " " << m_y1 << " Q " << m_cx << " " << m_cy << " " << m_x2 << " " << m_y2 << "\" " << SVG_STYLE_HOOK << " />";
    return s.str();
}

}
