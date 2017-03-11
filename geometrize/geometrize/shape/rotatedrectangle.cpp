#include "rotatedrectangle.h"

#include <cstdint>
#include <memory>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

RotatedRectangle::RotatedRectangle(const geometrize::Model& model) : m_model{model}
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    m_x1 = commonutil::randomRange(0, xBound - 1);
    m_y1 = commonutil::randomRange(0, yBound - 1);
    m_x2 = commonutil::clamp(m_x1 + commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1), 0, xBound);
    m_y2 = commonutil::clamp(m_y1 + commonutil::randomRange(0, commonutil::randomRange(0, 32) + 1), 0, yBound);
    m_angle = commonutil::randomRange(0, 360);
}

std::shared_ptr<geometrize::Shape> RotatedRectangle::clone() const
{
    std::shared_ptr<geometrize::RotatedRectangle> rect{std::make_shared<geometrize::RotatedRectangle>(m_model)};
    rect->m_x1 = m_x1;
    rect->m_y1 = m_y1;
    rect->m_x2 = m_x2;
    rect->m_y2 = m_y2;
    rect->m_angle = m_angle;
    return rect;
}

std::vector<geometrize::Scanline> RotatedRectangle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;



    return lines;
}

void RotatedRectangle::mutate()
{
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::int32_t r{commonutil::randomRange(0, 2)};
    switch(r) {
        case 0:
        {
            m_x1 = commonutil::clamp(m_x1 + commonutil::randomRange(-16, 16), 0, xBound);
            m_y1 = commonutil::clamp(m_y1 + commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 1:
        {
            m_x2 = commonutil::clamp(m_x2 + commonutil::randomRange(-16, 16), 0, xBound);
            m_y2 = commonutil::clamp(m_y2 + commonutil::randomRange(-16, 16), 0, yBound);
            break;
        }
        case 2:
        {
            m_angle = commonutil::clamp(m_angle + commonutil::randomRange(-16, 16), 0, 360);
            break;
        }
    }
}

geometrize::shapes::ShapeTypes RotatedRectangle::getType() const
{
    return geometrize::shapes::ShapeTypes::ROTATED_RECTANGLE;
}

std::vector<std::int32_t> RotatedRectangle::getRawShapeData() const
{
    return {
        ((std::min)(m_x1, m_x2)),
        ((std::min)(m_y1, m_y2)),
        ((std::max)(m_x1, m_x2)),
        ((std::max)(m_y1, m_y2)),
        m_angle
    };
}

std::string RotatedRectangle::getSvgShapeData(const std::string& attribs) const
{
    return "TODO";
}

}
