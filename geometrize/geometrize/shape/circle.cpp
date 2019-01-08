#include "circle.h"

#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"

namespace geometrize
{

Circle::Circle(const float x, const float y, const float r) : Shape()
{
    m_x = x;
    m_y = y;
    m_r = r;
}

std::shared_ptr<geometrize::Shape> Circle::clone() const
{
    std::shared_ptr<geometrize::Circle> circle{std::make_shared<geometrize::Circle>()};
    circle->m_x = m_x;
    circle->m_y = m_y;
    circle->m_r = m_r;
    circle->setup = setup;
    circle->mutate = mutate;
    circle->rasterize = rasterize;
    return circle;
}

void Circle::translate(const float x, const float y)
{
    m_x += x;
    m_y += y;
}

void Circle::scale(const float scaleFactor)
{
    m_r *= scaleFactor;
}

geometrize::ShapeTypes Circle::getType() const
{
    return geometrize::ShapeTypes::CIRCLE;
}

std::vector<float> Circle::getRawShapeData() const
{
    return { m_x, m_y, m_r };
}

std::string Circle::getSvgShapeData() const
{
    std::stringstream s;
    s << "<circle cx=\"" << m_x << "\" cy=\"" << m_y << "\" r=\"" << m_r << "\" " << SVG_STYLE_HOOK << " />";
    return s.str();
}

}
