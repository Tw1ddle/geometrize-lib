#include "rotatedellipse.h"

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

RotatedEllipse::RotatedEllipse(const geometrize::Model& model) : Shape{model}
{
    m_model->setupShape(*this);
}

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
    std::shared_ptr<geometrize::RotatedEllipse> ellipse{std::make_shared<geometrize::RotatedEllipse>(*m_model)};
    ellipse->m_x = m_x;
    ellipse->m_y = m_y;
    ellipse->m_rx = m_rx;
    ellipse->m_ry = m_ry;
    ellipse->m_angle = m_angle;
    return ellipse;
}

std::vector<geometrize::Scanline> RotatedEllipse::rasterize() const
{
    const std::int32_t w{m_model->getWidth()};
    const std::int32_t h{m_model->getHeight()};

    const std::uint32_t pointCount{20};
    std::vector<std::pair<float, float>> points;
    const float rads{m_angle * (3.141f / 180.0f)};
    const float c{std::cos(rads)};
    const float s{std::sin(rads)};

    for(std::uint32_t i = 0; i < pointCount; i++) {
        const float angle{((360.0f / pointCount) * i) * (3.141f / 180.0f)};
        const float crx{m_rx * std::cos(angle)};
        const float cry{m_ry * std::sin(angle)};
        points.push_back(std::make_pair(crx * c - cry * s + m_x, crx * s + cry * c + m_y));
    }

    std::vector<geometrize::Scanline> scanlines{geometrize::scanlinesForPolygon(points)};
    return geometrize::Scanline::trim(scanlines, w, h);
}

void RotatedEllipse::mutate()
{
    m_model->mutateShape(*this);
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
