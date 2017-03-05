#include "spline.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

Spline::Spline(const std::int32_t xBound, const std::int32_t yBound) : m_xBound(xBound), m_yBound(yBound), m_interpolationMode{SplineInterpolationMode::LINEAR}
{
    const std::pair<std::int32_t, std::int32_t> startingPoint{std::make_pair(commonutil::randomRange(0, m_xBound), commonutil::randomRange(0, m_yBound - 1))};
    for(std::int32_t i = 0; i < 4; i++) {
        const std::pair<std::int32_t, std::int32_t> point{
            commonutil::clamp(startingPoint.first + commonutil::randomRange(-32, 32), 0, m_xBound - 1),
            commonutil::clamp(startingPoint.second + commonutil::randomRange(-32, 32), 0, m_yBound - 1)
        };
        m_controlPoints.push_back(point);
    }
}

std::shared_ptr<geometrize::Shape> Spline::clone() const
{
    std::shared_ptr<geometrize::Spline> spline{std::make_shared<geometrize::Spline>(m_xBound, m_yBound)};
    spline->m_interpolationMode = m_interpolationMode;
    spline->m_controlPoints = m_controlPoints;
    return spline;
}

std::vector<geometrize::Scanline> Spline::rasterize() const
{
    std::vector<geometrize::Scanline> lines;

    for(std::int32_t i = 0; i < m_controlPoints.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> m0{i > 0 ? m_controlPoints[i - 1] : m_controlPoints[i]};
        const std::pair<std::int32_t, std::int32_t> m1{m_controlPoints[i]};
        const std::pair<std::int32_t, std::int32_t> m2{i < (m_controlPoints.size() - 1) ? m_controlPoints[i + 1] : m_controlPoints[i]};

        switch(m_interpolationMode) {
            case SplineInterpolationMode::CARDINAL:
            {
                break;
            }
            case SplineInterpolationMode::MONOTONE:
            {
                break;
            }
            case SplineInterpolationMode::BASIS:
            {
                break;
            }
            case SplineInterpolationMode::LINEAR:
            {
                const auto points{commonutil::bresenham(m1.first, m1.second, m2.first, m2.second)};
                for(const auto& point : points) {
                    lines.push_back(geometrize::Scanline(point.second, point.first, point.first, 0xFFFF));
                }
                break;
            }
            case SplineInterpolationMode::STEP_BEFORE:
            {
                break;
            }
            case SplineInterpolationMode::STEP_AFTER:
            {
                break;
            }
        }
    }

    return Scanline::trim(lines, m_xBound, m_yBound);
}

void Spline::mutate()
{
    const std::int32_t i{commonutil::randomRange(static_cast<std::size_t>(0), m_controlPoints.size() - 1)};

    std::pair<std::int32_t, std::int32_t> point{m_controlPoints[i]};
    point.first = commonutil::clamp(point.first + commonutil::randomRange(-64, 64), 0, m_xBound - 1);
    point.second = commonutil::clamp(point.second + commonutil::randomRange(-64, 64), 0, m_yBound - 1);

    m_controlPoints[i] = point;
}

geometrize::shapes::ShapeTypes Spline::getType() const
{
    return geometrize::shapes::ShapeTypes::SPLINE;
}

std::vector<std::int32_t> Spline::getRawShapeData() const
{
    std::vector<std::int32_t> data;
    data.push_back(static_cast<std::int32_t>(m_interpolationMode));

    for(std::int32_t i = 0; i < m_controlPoints.size(); i++) {
        data.push_back(m_controlPoints[i].first);
        data.push_back(m_controlPoints[i].second);
    }

    return data;
}

std::string Spline::getSvgShapeData() const
{
    return "TODO";
}

}
