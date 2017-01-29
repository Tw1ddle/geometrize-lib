#pragma once

#include <cstdint>

#include "shape.h"
#include "../util.h"

namespace geometrize
{

/**
 * @brief The Circle class represents a circle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Circle : public Shape
{
public:
    /**
     * @brief Circle Creates a new circle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Circle(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
    {
        // TODO
    }

    virtual geometrize::Shape* clone() const
    {
        return new Circle(m_xBound, m_yBound); // TODO
    }

    virtual std::vector<geometrize::Scanline> rasterize() const override
    {
        std::vector<geometrize::Scanline> lines;
        return lines; // TODO
    }

    virtual void mutate() override
    {
        // TODO
    }

    virtual geometrize::shapes::ShapeTypes getType() const override
    {
        return geometrize::shapes::ShapeTypes::CIRCLE;
    }

private:
    const std::uint32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::uint32_t m_yBound; ///< The y-bound of the whole canvas.
};

}
