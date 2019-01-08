#include "polyline.h"

#include <cstdint>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include "shape.h"

namespace geometrize
{

Polyline::Polyline(const std::vector<std::pair<float, float>>& points) : Shape()
{
    m_points = points;
}

std::shared_ptr<geometrize::Shape> Polyline::clone() const
{
    std::shared_ptr<geometrize::Polyline> polyline{std::make_shared<geometrize::Polyline>()};
    polyline->m_points = m_points;
    polyline->setup = setup;
    polyline->mutate = mutate;
    polyline->rasterize = rasterize;
    return polyline;
}

void Polyline::translate(const float x, const float y)
{
    for(auto& point : m_points) {
        point.first += x;
        point.second += y;
    }
}

void Polyline::scale(const float scaleFactor)
{
    // TODO
}

geometrize::ShapeTypes Polyline::getType() const
{
    return geometrize::ShapeTypes::POLYLINE;
}

std::vector<float> Polyline::getRawShapeData() const
{
    std::vector<float> data;
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
