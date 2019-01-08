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
    RotatedEllipse() = default;
    RotatedEllipse(float x, float y, float rx, float ry, float angle);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual geometrize::ShapeTypes getType() const override;

    float m_x; ///< x-coordinate.
    float m_y; ///< y-coordinate.
    float m_rx; ///< x-radius.
    float m_ry; ///< y-radius.
    float m_angle; ///< Rotation angle.
};

}
