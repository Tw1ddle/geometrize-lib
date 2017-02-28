#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The RotatedRectangle class represents a rotated rectangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class RotatedRectangle : public Shape
{
public:
    /**
     * @brief RotatedRectangle Creates a new rotated rectangle.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    RotatedRectangle(std::int32_t xBound, std::int32_t yBound);

    virtual std::shared_ptr<geometrize::Shape> clone() const;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getShapeData() const override;

private:
    const std::int32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::int32_t m_yBound; ///< The y-bound of the whole canvas.

    std::int32_t m_x1; ///< Left coordinate.
    std::int32_t m_y1; ///< Top coordinate.
    std::int32_t m_x2; ///< Right coordinate.
    std::int32_t m_y2; ///< Bottom coordinate.
    std::int32_t m_angle; ///< Rotation angle.
};

}
