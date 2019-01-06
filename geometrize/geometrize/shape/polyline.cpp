#include "polyline.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"
#include "../rasterizer/rasterizer.h"

namespace geometrize
{

Polyline::Polyline(const geometrize::Model& model) : Shape{model}
{
    m_model->setupShape(*this);
}

Polyline::Polyline(const std::vector<std::pair<std::int32_t, std::int32_t>>& points) : Shape()
{
    m_points = points;
}

std::shared_ptr<geometrize::Shape> Polyline::clone() const
{
    std::shared_ptr<geometrize::Polyline> polyline{std::make_shared<geometrize::Polyline>(*m_model)};
    polyline->m_points = m_points;
    return polyline;
}

std::vector<geometrize::Scanline> Polyline::rasterize() const
{
    const std::int32_t xBound{m_model->getWidth()};
    const std::int32_t yBound{m_model->getHeight()};

    std::vector<geometrize::Scanline> lines;

    for(std::size_t i = 0; i < m_points.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> p0{m_points[i]};
        const std::pair<std::int32_t, std::int32_t> p1{i < (m_points.size() - 1) ? m_points[i + 1] : m_points[i]};

        const std::vector<std::pair<std::int32_t, std::int32_t>> points{geometrize::bresenham(p0.first, p0.second, p1.first, p1.second)};
        for(const auto& point : points) {
            lines.push_back(geometrize::Scanline(point.second, point.first, point.first));
        }
    }

    return Scanline::trim(lines, xBound, yBound);
}

void Polyline::mutate()
{
    m_model->mutateShape(*this);
}

geometrize::ShapeTypes Polyline::getType() const
{
    return geometrize::ShapeTypes::POLYLINE;
}

std::vector<std::int32_t> Polyline::getRawShapeData() const
{
    std::vector<std::int32_t> data;
    for(std::size_t i = 0; i < m_points.size(); i++) {
        data.push_back(m_points[i].first);
        data.push_back(m_points[i].second);
    }

    return data;
}

std::string Polyline::getSvgShapeData() const
{
    std::stringstream s;
    s << "<polyline points=\"";
    for(std::size_t i = 0; i < m_points.size(); i++) {
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
