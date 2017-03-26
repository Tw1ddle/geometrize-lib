#include "polyline.h"

#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"
#include "shapeutil.h"

namespace geometrize
{

Polyline::Polyline(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(commonutil::randomRange(0, xBound), commonutil::randomRange(0, yBound - 1))};
    for(std::int32_t i = 0; i < 4; i++) {
        const std::pair<std::int32_t, std::int32_t> point{
            commonutil::clamp(startingPoint.first + commonutil::randomRange(-32, 32), 0, xBound - 1),
            commonutil::clamp(startingPoint.second + commonutil::randomRange(-32, 32), 0, yBound - 1)
        };
        m_points.push_back(point);
    }
}

std::shared_ptr<geometrize::Shape> Polyline::clone() const
{
    std::shared_ptr<geometrize::Polyline> polyline{std::make_shared<geometrize::Polyline>(m_model)};
    polyline->m_points = m_points;
    return polyline;
}

std::vector<geometrize::Scanline> Polyline::rasterize() const
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    std::vector<geometrize::Scanline> lines;

    for(std::int32_t i = 0; i < m_points.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> p0{m_points[i]};
        const std::pair<std::int32_t, std::int32_t> p1{i < (m_points.size() - 1) ? m_points[i + 1] : m_points[i]};

        const auto points{geometrize::bresenham(p0.first, p0.second, p1.first, p1.second)};
        for(const auto& point : points) {
            lines.push_back(geometrize::Scanline(point.second, point.first, point.first));
        }
    }

    return Scanline::trim(lines, xBound, yBound);
}

void Polyline::mutate()
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};
    const std::int32_t i{commonutil::randomRange(static_cast<std::size_t>(0), m_points.size() - 1)};

    std::pair<std::int32_t, std::int32_t> point{m_points[i]};
    point.first = commonutil::clamp(point.first + commonutil::randomRange(-64, 64), 0, xBound - 1);
    point.second = commonutil::clamp(point.second + commonutil::randomRange(-64, 64), 0, yBound - 1);

    m_points[i] = point;
}

geometrize::ShapeTypes Polyline::getType() const
{
    return geometrize::ShapeTypes::POLYLINE;
}

std::vector<std::int32_t> Polyline::getRawShapeData() const
{
    std::vector<std::int32_t> data;
    for(std::int32_t i = 0; i < m_points.size(); i++) {
        data.push_back(m_points[i].first);
        data.push_back(m_points[i].second);
    }

    return data;
}

std::string Polyline::getSvgShapeData() const
{
    std::stringstream s;
    s << "<polyline points=\"";
    for(std::int32_t i = 0; i < m_points.size(); i++) {
        s << m_points[i].first << "," << m_points[i].second;
        if(i != m_points.size() - 1) {
            s << " ";
        }
    }
    s << "\" ";

    s << SVG_STYLE_HOOK << " ";
    s << "/>";

    return s.str();
}

}
