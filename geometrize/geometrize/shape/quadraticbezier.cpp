#include "quadraticbezier.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"
#include "shapeutil.h"

namespace geometrize
{

QuadraticBezier::QuadraticBezier(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    m_x1 = commonutil::randomRange(0, commonutil::randomRange(0, xBound - 1));
    m_y1 = commonutil::randomRange(0, commonutil::randomRange(0, yBound - 1));
    m_cx = commonutil::randomRange(0, commonutil::randomRange(0, xBound - 1));
    m_cy = commonutil::randomRange(0, commonutil::randomRange(0, yBound - 1));
    m_x2 = commonutil::randomRange(0, commonutil::randomRange(0, xBound - 1));
    m_y2 = commonutil::randomRange(0, commonutil::randomRange(0, yBound - 1));
}

std::shared_ptr<geometrize::Shape> QuadraticBezier::clone() const
{
    std::shared_ptr<geometrize::QuadraticBezier> bezier{std::make_shared<geometrize::QuadraticBezier>(m_model)};
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

    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    std::vector<std::pair<std::int32_t, std::int32_t>> points;
    const std::uint32_t pointCount{20};
    for(std::uint32_t i = 0; i <= pointCount; i++) {
        const float t{static_cast<float>(i) / static_cast<float>(pointCount)};
        const std::int32_t x{static_cast<std::int32_t>((1 - t) * (1 - t) * m_x1 + 2 * (1 - t) * t * m_cx + t * t + m_x2)};
        const std::int32_t y{static_cast<std::int32_t>((1 - t) * (1 - t) * m_y1 + 2 * (1 - t) * t * m_cy + t * t + m_y2)};
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
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::int32_t r{commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            m_cx = commonutil::clamp(m_cx + commonutil::randomRange(-8, 8), 0, xBound - 1);
            m_cy = commonutil::clamp(m_cy + commonutil::randomRange(-8, 8), 0, yBound - 1);
            break;
        }
        case 1:
        {
            m_x1 = commonutil::clamp(m_x1 + commonutil::randomRange(-8, 8), 1, xBound - 1);
            m_y1 = commonutil::clamp(m_y1 + commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
        case 2:
        {
            m_x2 = commonutil::clamp(m_x2 + commonutil::randomRange(-8, 8), 1, xBound - 1);
            m_y2 = commonutil::clamp(m_y2 + commonutil::randomRange(-8, 8), 1, yBound - 1);
            break;
        }
    }
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
