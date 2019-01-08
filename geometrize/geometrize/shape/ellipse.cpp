#include "ellipse.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <sstream>

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
