#include "spline.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Spline::Spline(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(commonutil::randomRange(0, xBound), commonutil::randomRange(0, yBound - 1))};
    for(std::int32_t i = 0; i < 4; i++) {
        const std::pair<std::int32_t, std::int32_t> point{
            commonutil::clamp(startingPoint.first + commonutil::randomRange(-32, 32), 0, xBound - 1),
            commonutil::clamp(startingPoint.second + commonutil::randomRange(-32, 32), 0, yBound - 1)
        };
        m_controlPoints.push_back(point);
    }
}

std::shared_ptr<geometrize::Shape> Spline::clone() const
{
    std::shared_ptr<geometrize::Spline> spline{std::make_shared<geometrize::Spline>(m_model)};
    spline->m_controlPoints = m_controlPoints;
    return spline;
}

std::vector<geometrize::Scanline> Spline::rasterize() const
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    std::vector<geometrize::Scanline> lines;

    for(std::int32_t i = 0; i < m_controlPoints.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> m0{i > 0 ? m_controlPoints[i - 1] : m_controlPoints[i]};
        const std::pair<std::int32_t, std::int32_t> m1{m_controlPoints[i]};
        const std::pair<std::int32_t, std::int32_t> m2{i < (m_controlPoints.size() - 1) ? m_controlPoints[i + 1] : m_controlPoints[i]};

        const auto points{commonutil::bresenham(m1.first, m1.second, m2.first, m2.second)};
        for(const auto& point : points) {
            lines.push_back(geometrize::Scanline(point.second, point.first, point.first, 0xFFFF));
        }
    }

    return Scanline::trim(lines, xBound, yBound);
}

void Spline::mutate()
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};
    const std::int32_t i{commonutil::randomRange(static_cast<std::size_t>(0), m_controlPoints.size() - 1)};

    std::pair<std::int32_t, std::int32_t> point{m_controlPoints[i]};
    point.first = commonutil::clamp(point.first + commonutil::randomRange(-64, 64), 0, xBound - 1);
    point.second = commonutil::clamp(point.second + commonutil::randomRange(-64, 64), 0, yBound - 1);

    m_controlPoints[i] = point;
}

geometrize::shapes::ShapeTypes Spline::getType() const
{
    return geometrize::shapes::ShapeTypes::SPLINE;
}

std::vector<std::int32_t> Spline::getRawShapeData() const
{
    std::vector<std::int32_t> data;
    for(std::int32_t i = 0; i < m_controlPoints.size(); i++) {
        data.push_back(m_controlPoints[i].first);
        data.push_back(m_controlPoints[i].second);
    }

    return data;
}

std::string Spline::getSvgShapeData(const std::string& attribs) const
{
    return "TODO";
}

}
