#include "rotatedellipse.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"

namespace geometrize
{

RotatedEllipse::RotatedEllipse(const float x, const float y, const float rx, const float ry, const float angle) : Shape()
{
    m_x = x;
    m_y = y;
    m_rx = rx;
    m_ry = ry;
    m_angle = angle;
}

std::shared_ptr<geometrize::Shape> RotatedEllipse::clone() const
{
    std::shared_ptr<geometrize::RotatedEllipse> ellipse{std::make_shared<geometrize::RotatedEllipse>()};
    ellipse->m_x = m_x;
    ellipse->m_y = m_y;
    ellipse->m_rx = m_rx;
    ellipse->m_ry = m_ry;
    ellipse->m_angle = m_angle;
    ellipse->setup = setup;
    ellipse->mutate = mutate;
    ellipse->rasterize = rasterize;
    return ellipse;
}

void RotatedEllipse::translate(const float x, const float y)
{
    m_x += x;
    m_y += y;
}

void RotatedEllipse::scale(const float scaleFactor)
{
    // TODO
}

geometrize::ShapeTypes RotatedEllipse::getType() const
{
    return geometrize::ShapeTypes::ROTATED_ELLIPSE;
}

std::vector<float> RotatedEllipse::getRawShapeData() const
{
    return { m_x, m_y, m_rx, m_ry, m_angle };
}

std::string RotatedEllipse::getSvgShapeData() const
{
    std::stringstream s;
    s << "<g transform=\"translate(" << m_x << " " << m_y << ") rotate(" << m_angle << ") scale(" << m_rx << " " << m_ry << ")\">"
      << "<ellipse cx=\"" << 0 << "\" cy=\"" << 0 << "\" rx=\"" << 1 << "\" ry=\"" << 1 << "\" " << SVG_STYLE_HOOK << " />"
      << "</g>";
    return s.str();
}

}
