#include "circle.h"

#include <cstdint>
#include <memory>
#include <sstream>

#include "shape.h"
#include "../model.h"
#include "../commonutil.h"

namespace geometrize
{

Circle::Circle(const geometrize::Model& model) : Shape{model}
{
    m_model.setupShape(*this);
}

std::shared_ptr<geometrize::Shape> Circle::clone() const
{
    std::shared_ptr<geometrize::Circle> circle{std::make_shared<geometrize::Circle>(m_model)};
    circle->m_x = m_x;
    circle->m_y = m_y;
    circle->m_r = m_r;
    return circle;
}

std::vector<geometrize::Scanline> Circle::rasterize() const
{
    std::vector<geometrize::Scanline> lines;
    const std::int32_t xBound{m_model.getWidth()};
    const std::int32_t yBound{m_model.getHeight()};

    const std::int32_t r{static_cast<std::int32_t>(m_r)};
    for(std::int32_t y = -r; y <= r; y++) {
        std::vector<std::int32_t> xScan;
        for(std::int32_t x = -r; x <= r; x++) {
            if(x * x + y * y <= r * r) {
                xScan.push_back(x);
            }
        }

        if(!xScan.empty()) {
            const std::int32_t fy{m_y + y};
            const std::int32_t x1{commonutil::clamp(m_x + xScan.front(), 0, xBound - 1)};
            const std::int32_t x2{commonutil::clamp(m_x + xScan.back(), 0, xBound - 1)};
            lines.push_back(geometrize::Scanline(fy, x1, x2));
        }
    }

    return geometrize::Scanline::trim(lines, xBound, yBound);
}

void Circle::mutate()
{
    m_model.mutateShape(*this);
}

geometrize::ShapeTypes Circle::getType() const
{
    return geometrize::ShapeTypes::CIRCLE;
}

std::vector<std::int32_t> Circle::getRawShapeData() const
{
    return { m_x, m_y, m_r };
}

std::string Circle::getSvgShapeData() const
{
    std::stringstream s;
    s << "<circle cx=\"" << m_x << "\" cy=\"" << m_y << "\" r=\"" << m_r << "\" " << SVG_STYLE_HOOK << " />";
    return s.str();
}

}
