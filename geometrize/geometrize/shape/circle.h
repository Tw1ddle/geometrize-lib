#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The Circle class represents a circle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Circle : public Shape
{
public:
    Circle() = default;
    Circle(float x, float y, float r);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual geometrize::ShapeTypes getType() const override;

    float m_x; ///< x-coordinate.
    float m_y; ///< y-coordinate.
    float m_r; ///< Radius.
};

}
