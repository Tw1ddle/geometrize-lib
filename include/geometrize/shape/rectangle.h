#pragma once

#include <algorithm>
#include <cstdint>

#include "shape.h"
#include "../util.h"

namespace geometrize
{

/**
 * @brief The Rectangle class represents a rectangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Rectangle : public Shape
{
public:
    /**
     * @brief Rectangle Creates a new rectangle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Rectangle(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
    {
        m_x1 = util::Random::randomRange(0, m_xBound);
        m_y1 = util::Random::randomRange(0, m_yBound);
        m_x2 = util::clamp(m_x1 + util::Random::randomRange(0, util::Random::randomRange(0, 32) + 1), 0U, m_xBound);
        m_y2 = util::clamp(m_y1 + util::Random::randomRange(0, util::Random::randomRange(0, 32) + 1), 0U, m_yBound);
    }

    virtual Shape* clone() const
    {
        Rectangle* rect{new Rectangle(m_xBound, m_yBound)};
        rect->m_x1 = m_x1;
        rect->m_y1 = m_y1;
        rect->m_x2 = m_x2;
        rect->m_y2 = m_y2;
        return rect;
    }

    virtual std::vector<Scanline> rasterize() const override
    {
        std::vector<Scanline> lines;
        for(std::uint32_t y = m_y1; y < m_y2; y++) {
            if(m_x1 != m_x2) {
                lines.push_back(Scanline(y, (std::min)(m_x1, m_x2), (std::max)(m_x1, m_x2), 0xFFFF));
            }
        }
        return lines;
    }

    virtual void mutate() override
    {
        const int r = util::Random::randomRange(0, 1);

        switch(r) {
        case 0:
            m_x1 = util::clamp(m_x1 + util::Random::randomRange(-16, 16), 0U, m_xBound - 1);
            m_y1 = util::clamp(m_y1 + util::Random::randomRange(-16, 16), 0U, m_yBound - 1);
        case 1:
            m_x2 = util::clamp(m_x2 + util::Random::randomRange(-16, 16), 0U, m_xBound - 1);
            m_y2 = util::clamp(m_y2 + util::Random::randomRange(-16, 16), 0U, m_yBound - 1);
        }
    }

    virtual shapes::ShapeTypes getType() const override
    {
        return shapes::ShapeTypes::RECTANGLE;
    }

private:
    const std::uint32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::uint32_t m_yBound; ///< The y-bound of the whole canvas.

    std::uint32_t m_x1; ///< Left coordinate.
    std::uint32_t m_y1; ///< Top coordinate.
    std::uint32_t m_x2; ///< Right coordinate.
    std::uint32_t m_y2; ///< Bottom coordinate.
};

}
