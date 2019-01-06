#include "rotatedrectangle.h"

#include <cmath>
#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"
#include "../rasterizer/rasterizer.h"

namespace geometrize
{

RotatedRectangle::RotatedRectangle(const geometrize::Model& model) : Shape{model}
{
    m_model->setupShape(*this);
}

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
    std::shared_ptr<geometrize::RotatedRectangle> rect{std::make_shared<geometrize::RotatedRectangle>(*m_model)};
    rect->m_x1 = m_x1;
    rect->m_y1 = m_y1;
    rect->m_x2 = m_x2;
    rect->m_y2 = m_y2;
    rect->m_angle = m_angle;
    return rect;
}

std::vector<geometrize::Scanline> RotatedRectangle::rasterize() const
{
    std::vector<geometrize::Scanline> scanlines{geometrize::scanlinesForPolygon(getCornerPoints())};
    return geometrize::Scanline::trim(scanlines, m_model->getWidth(), m_model->getHeight());
}

void RotatedRectangle::translate(const float x, const float y)
{
    m_x1 += x;
    m_y1 += y;
    m_x2 += x;
    m_y2 += y;
}

void RotatedRectangle::mutate()
{
    m_model->mutateShape(*this);
}

void RotatedRectangle::scale(const float scaleFactor)
{
    // TODO
}

std::vector<std::pair<float, float>> RotatedRectangle::getCornerPoints() const
{
    const float x1{(std::min)(m_x1, m_x2)};
    const float x2{(std::max)(m_x1, m_x2)};
    const float y1{(std::min)(m_y1, m_y2)};
    const float y2{(std::max)(m_y1, m_y2)};

    const float cx{(x2 + x1) / 2.0f};
    const float cy{(y2 + y1) / 2.0f};

    const float ox1{x1 - cx};
    const float ox2{x2 - cx};
    const float oy1{y1 - cy};
    const float oy2{y2 - cy};

    const float rads{m_angle * 3.141f / 180.0f};
    const float c{std::cos(rads)};
    const float s{std::sin(rads)};

    const std::pair<float, float> ul{ox1 * c - oy1 * s + cx, ox1 * s + oy1 * c + cy};
    const std::pair<float, float> bl{ox1 * c - oy2 * s + cx, ox1 * s + oy2 * c + cy};
    const std::pair<float, float> ur{ox2 * c - oy1 * s + cx, ox2 * s + oy1 * c + cy};
    const std::pair<float, float> br{ox2 * c - oy2 * s + cx, ox2 * s + oy2 * c + cy};

    return {ul, ur, br, bl};
}

geometrize::ShapeTypes RotatedRectangle::getType() const
{
    return geometrize::ShapeTypes::ROTATED_RECTANGLE;
}

std::vector<float> RotatedRectangle::getRawShapeData() const
{
    return { ((std::min)(m_x1, m_x2)), ((std::min)(m_y1, m_y2)), ((std::max)(m_x1, m_x2)), ((std::max)(m_y1, m_y2)), m_angle};
}

std::string RotatedRectangle::getSvgShapeData() const
{
    const std::vector<std::pair<float, float>> points{getCornerPoints()};
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
