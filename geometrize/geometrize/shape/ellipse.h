#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The Ellipse class represents an ellipse.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Ellipse : public Shape
{
public:
    Ellipse() = default;
    Ellipse(float x, float y, float rx, float ry);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual geometrize::ShapeTypes getType() const override;

    float m_x; ///< x-coordinate.
    float m_y; ///< y-coordinate.
    float m_rx; ///< x-radius.
    float m_ry; ///< y-radius.
};

}
