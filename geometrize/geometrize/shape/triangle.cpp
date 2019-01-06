#include "triangle.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"
#include "../rasterizer/rasterizer.h"

namespace geometrize
{

Triangle::Triangle(const geometrize::Model& model) : Shape{model}
{
    m_model->setupShape(*this);
}

Triangle::Triangle(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3) : Shape()
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
    m_x3 = x3;
    m_y3 = y3;
}

std::shared_ptr<geometrize::Shape> Triangle::clone() const
{
    std::shared_ptr<geometrize::Triangle> triangle{std::make_shared<geometrize::Triangle>(*m_model)};
    triangle->m_x1 = m_x1;
    triangle->m_y1 = m_y1;
    triangle->m_x2 = m_x2;
    triangle->m_y2 = m_y2;
    triangle->m_x3 = m_x3;
    triangle->m_y3 = m_y3;
    return triangle;
}

std::vector<geometrize::Scanline> Triangle::rasterize() const
{
    std::vector<geometrize::Scanline> scanlines{geometrize::scanlinesForPolygon({{static_cast<std::int32_t>(m_x1), static_cast<std::int32_t>(m_y1)}, {static_cast<std::int32_t>(m_x2), static_cast<std::int32_t>(m_y2)}, {static_cast<std::int32_t>(m_x3), static_cast<std::int32_t>(m_y3)}})};
    return Scanline::trim(scanlines, m_model->getWidth(), m_model->getHeight());
}

void Triangle::mutate()
{
    m_model->mutateShape(*this);
}

void Triangle::translate(const float x, const float y)
{
    m_x1 += x;
    m_y1 += y;
    m_x2 += x;
    m_y2 += y;
    m_x3 += x;
    m_y3 += y;
}

void Triangle::scale(const float scaleFactor)
{
    // TODO
}

geometrize::ShapeTypes Triangle::getType() const
{
    return ShapeTypes::TRIANGLE;
}

std::vector<float> Triangle::getRawShapeData() const
{
    return { m_x1, m_y1, m_x2, m_y2, m_x3, m_y3 };
}

std::string Triangle::getSvgShapeData() const
{
    std::stringstream s;
    s << "<polygon points=\"" << m_x1 << "," << m_y1 << " " << m_x2 << "," << m_y2 << " " << m_x3 << "," << m_y3 << "\" " << SVG_STYLE_HOOK << " " << "/>";
    return s.str();
}

}
