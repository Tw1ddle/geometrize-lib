#include "quadraticbezier.h"

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

QuadraticBezier::QuadraticBezier(const geometrize::Model& model) : Shape{model}
{
    m_model->setupShape(*this);
}

QuadraticBezier::QuadraticBezier(const std::int32_t cx, const std::int32_t cy, const std::int32_t x1, const std::int32_t y1, const std::int32_t x2, const std::int32_t y2) : Shape()
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
    std::shared_ptr<geometrize::QuadraticBezier> bezier{std::make_shared<geometrize::QuadraticBezier>(*m_model)};
    bezier->m_x1 = m_x1;
    bezier->m_y1 = m_y1;
    bezier->m_cx = m_cx;
    bezier->m_cy = m_cy;
    bezier->m_x2 = m_x2;
    bezier->m_y2 = m_y2;
    return bezier;
}

std::vector<geometrize::Scanline> QuadraticBezier::rasterize() const
{
    std::vector<geometrize::Scanline> scanlines;

    const std::int32_t xBound{m_model->getWidth()};
    const std::int32_t yBound{m_model->getHeight()};

    std::vector<std::pair<std::int32_t, std::int32_t>> points;
    const std::uint32_t pointCount{20};
    for(std::uint32_t i = 0; i <= pointCount; i++) {
        const float t{static_cast<float>(i) / static_cast<float>(pointCount)};
        const float tp{1 - t};
        const std::int32_t x{static_cast<std::int32_t>(tp * (tp * m_x1 + (t * m_cx)) + t * ((tp * m_cx) + (t * m_x2)))};
        const std::int32_t y{static_cast<std::int32_t>(tp * (tp * m_y1 + (t * m_cy)) + t * ((tp * m_cy) + (t * m_y2)))};
        points.push_back(std::make_pair(x, y));
    }

    for(std::uint32_t i = 0; i < points.size() - 1; i++) {
        const std::pair<std::int32_t, std::int32_t> p0{points[i]};
        const std::pair<std::int32_t, std::int32_t> p1{points[i + 1]};

        const std::vector<std::pair<std::int32_t, std::int32_t>> points{geometrize::bresenham(p0.first, p0.second, p1.first, p1.second)};
        for(const std::pair<std::int32_t, std::int32_t>& point : points) {
            scanlines.push_back(geometrize::Scanline(point.second, point.first, point.first));
        }
    }

    return Scanline::trim(scanlines, xBound, yBound);
}

void QuadraticBezier::mutate()
{
    m_model->mutateShape(*this);
}

geometrize::ShapeTypes QuadraticBezier::getType() const
{
    return geometrize::ShapeTypes::QUADRATIC_BEZIER;
}

std::vector<std::int32_t> QuadraticBezier::getRawShapeData() const
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
