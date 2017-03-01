#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The RotatedEllipse class represents a rotated ellipse.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class RotatedEllipse : public Shape
{
public:
    /**
     * @brief RotatedEllipse Creates a new rotated ellipse.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    RotatedEllipse(std::int32_t xBound, std::int32_t yBound);

    virtual std::shared_ptr<geometrize::Shape> clone() const;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

private:
    const std::int32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::int32_t m_yBound; ///< The y-bound of the whole canvas.

    std::int32_t m_x; ///< x-coordinate.
    std::int32_t m_y; ///< y-coordinate.
    std::int32_t m_rx; ///< x-radius.
    std::int32_t m_ry; ///< y-radius.
    std::int32_t m_angle; ///< Rotation angle.
};

}
