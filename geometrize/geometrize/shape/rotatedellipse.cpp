#include "rotatedellipse.h"

#include <cstdint>
#include <math.h>
#include <memory>
#include <sstream>

#include "shape.h"
#include "shapeutil.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

RotatedEllipse::RotatedEllipse(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    m_x = commonutil::randomRange(0, xBound - 1);
    m_y = commonutil::randomRange(0, yBound - 1);
    m_rx = commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1);
    m_ry = commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1);
    m_angle = commonutil::randomRange(0, 360);
}

std::shared_ptr<geometrize::Shape> RotatedEllipse::clone() const
{
    std::shared_ptr<geometrize::RotatedEllipse> ellipse{std::make_shared<geometrize::RotatedEllipse>(m_model)};
    ellipse->m_x = m_x;
    ellipse->m_y = m_y;
    ellipse->m_rx = m_rx;
    ellipse->m_ry = m_ry;
    ellipse->m_angle = m_angle;
    return ellipse;
}

std::vector<geometrize::Scanline> RotatedEllipse::rasterize() const
{
    const std::int32_t w{m_model.getWidth()};
    const std::int32_t h{m_model.getHeight()};

    const std::uint32_t pointCount{20};
    std::vector<std::pair<std::int32_t, std::int32_t>> points;
    const float rads{m_angle * 3.141f / 180.0f};
    const float c{cos(rads)};
    const float s{sin(rads)};

    for(std::uint32_t i = 0; i < pointCount; i++) {
        const float angle{((360.0f / pointCount) * i) * (3.141f / 180.0f)};
        std::pair<std::int32_t, std::int32_t> point{std::make_pair(m_rx * 2 * cos(angle), m_ry * 2 * sin(angle))};

        point.first = static_cast<std::int32_t>(point.first * c - point.second * s + m_x);
        point.second = static_cast<std::int32_t>(point.first * s + point.second * c + m_y);

        points.push_back(point);
    }

    return Scanline::trim(geometrize::scanlinesForPolygon(points), w, h);
}

void RotatedEllipse::mutate()
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::int32_t r{commonutil::randomRange(0, 3)};
    switch(r) {
        case 0:
        {
            m_x = commonutil::clamp(m_x + commonutil::randomRange(-16, 16), 0, xBound - 1);
            m_y = commonutil::clamp(m_y + commonutil::randomRange(-16, 16), 0, yBound - 1);
            break;
        }
        case 1:
        {
            m_rx = commonutil::clamp(m_rx + commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 2:
        {
            m_ry = commonutil::clamp(m_ry + commonutil::randomRange(-16, 16), 1, xBound - 1);
            break;
        }
        case 3:
        {
            m_angle = commonutil::clamp(m_angle + commonutil::randomRange(-16, 16), 0, 360);
            break;
        }
    }
}

geometrize::ShapeTypes RotatedEllipse::getType() const
{
    return geometrize::ShapeTypes::ROTATED_ELLIPSE;
}

std::vector<std::int32_t> RotatedEllipse::getRawShapeData() const
{
    return { m_x, m_y, m_rx, m_ry, m_angle };
}

std::string RotatedEllipse::getSvgShapeData() const
{
    std::stringstream s;
    s << "<ellipse "
      << "cx=\"" << m_x << "\" "
      << "cy=\"" << m_y << "\" "
      << "rx=\"" << m_rx << "\" "
      << "ry=\"" << m_ry << "\" "
      << "transform=\"rotate(" << m_angle << ")" << "\" "
      << SVG_STYLE_HOOK << " "
      << "/>";

    return s.str();
}

}
