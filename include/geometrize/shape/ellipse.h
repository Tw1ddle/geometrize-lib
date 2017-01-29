#pragma once

#include <cstdint>

#include "shape.h"
#include "../util.h"

namespace geometrize
{

/**
 * @brief The Ellipse class represents an ellipse.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Ellipse : public Shape
{
public:
    /**
     * @brief Ellipse Creates a new ellipse.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Ellipse(const std::uint32_t xBound, const std::uint32_t yBound) : m_xBound(xBound), m_yBound(yBound)
    {
        // TODO
    }

    virtual Shape* clone() const
    {
        return new Ellipse(m_xBound, m_yBound); // TODO
    }

    virtual std::vector<Scanline> rasterize() const override
    {
        std::vector<Scanline> lines;
        return lines; // TODO
    }

    virtual void mutate() override
    {
        // TODO
    }

    virtual shapes::ShapeTypes getType() const override
    {
        return shapes::ShapeTypes::ELLIPSE;
    }

    virtual std::vector<std::int32_t> getShapeData() const override
    {
        return {};
    }

private:
    const std::uint32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::uint32_t m_yBound; ///< The y-bound of the whole canvas.
};

}
