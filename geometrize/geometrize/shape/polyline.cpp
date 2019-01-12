#include "polyline.h"

#include <cstdint>
#include <memory>
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

geometrize::ShapeTypes Polyline::getType() const
{
    return geometrize::ShapeTypes::POLYLINE;
}

}
