#include "rotatedellipse.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../commonutil.h"

namespace geometrize
{

RotatedEllipse::RotatedEllipse(const std::int32_t xBound, const std::int32_t yBound) : m_xBound(xBound), m_yBound(yBound)
{
    m_x = commonutil::randomRange(0, m_xBound - 1);
    m_y = commonutil::randomRange(0, m_yBound - 1);
    m_rx = commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1);
    m_ry = commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1);
    m_angle = commonutil::randomRange(0, 360);
}

std::shared_ptr<geometrize::Shape> RotatedEllipse::clone() const
{
    std::shared_ptr<geometrize::RotatedEllipse> ellipse{std::make_shared<geometrize::RotatedEllipse>(m_xBound, m_yBound)};
    ellipse->m_x = m_x;
    ellipse->m_y = m_y;
    ellipse->m_rx = m_rx;
    ellipse->m_ry = m_ry;
    ellipse->m_angle = m_angle;
    return ellipse;
}

std::vector<geometrize::Scanline> RotatedEllipse::rasterize() const
{
    std::vector<geometrize::Scanline> lines;



    return lines;
}

void RotatedEllipse::mutate()
{
    const std::int32_t r{commonutil::randomRange(0, 3)};
    switch(r) {
        case 0:
        {
            m_y = commonutil::clamp(m_x + commonutil::randomRange(-16, 16), 0, m_xBound - 1);
            m_y = commonutil::clamp(m_y + commonutil::randomRange(-16, 16), 0, m_yBound - 1);
            break;
        }
        case 1:
        {
            m_rx = commonutil::clamp(m_rx + commonutil::randomRange(-16, 16), 1, m_xBound - 1);
            break;
        }
        case 2:
        {
            m_ry = commonutil::clamp(m_ry + commonutil::randomRange(-16, 16), 1, m_xBound - 1);
            break;
        }
        case 3:
        {
            m_angle = commonutil::clamp(m_angle + commonutil::randomRange(-16, 16), 0, 360);
            break;
        }
    }
}

geometrize::shapes::ShapeTypes RotatedEllipse::getType() const
{
    return geometrize::shapes::ShapeTypes::ROTATED_ELLIPSE;
}

std::vector<std::int32_t> RotatedEllipse::getRawShapeData() const
{
    return { m_x, m_y, m_rx, m_ry, m_angle };
}

std::string RotatedEllipse::getSvgShapeData() const
{
    return "TODO";
}

}
