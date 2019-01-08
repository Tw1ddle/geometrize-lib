#include "rotatedrectangle.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../rasterizer/rasterizer.h"

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

void RotatedRectangle::translate(const float x, const float y)
{
    m_x1 += x;
    m_y1 += y;
    m_x2 += x;
    m_y2 += y;
}

void RotatedRectangle::scale(const float scaleFactor)
{
    // TODO
}

geometrize::ShapeTypes RotatedRectangle::getType() const
{
    return geometrize::ShapeTypes::ROTATED_RECTANGLE;
}

std::vector<float> RotatedRectangle::getRawShapeData() const
{
    return { ((std::fmin)(m_x1, m_x2)), ((std::fmin)(m_y1, m_y2)), ((std::fmax)(m_x1, m_x2)), ((std::fmax)(m_y1, m_y2)), m_angle};
}

std::string RotatedRectangle::getSvgShapeData() const
{
    const std::vector<std::pair<float, float>> points{geometrize::getCornerPoints(*this)};
    std::stringstream s;
    s << "<polygon points=\"";
    for(std::size_t i = 0; i < points.size(); i++) {
        s << points[i].first << "," << points[i].second;
        if(i != points.size() - 1) {
            s << " ";
        }
    }
    s << "\" " << SVG_STYLE_HOOK << "/>";
    return s.str();
}

}
