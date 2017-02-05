#pragma once

#include <cstdint>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The Triangle class represents a triangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Triangle : public Shape
{
public:
    /**
     * @brief Triangle Creates a new triangle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Triangle(const std::uint32_t xBound, const std::uint32_t yBound);

    virtual geometrize::Shape* clone() const;
    virtual std::vector<Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getShapeData() const override;

private:
    const std::uint32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::uint32_t m_yBound; ///< The y-bound of the whole canvas.
};

}
