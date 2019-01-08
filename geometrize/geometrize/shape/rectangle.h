#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The Rectangle class represents a rectangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Rectangle : public Shape
{
public:
    Rectangle() = default;
    Rectangle(float x1, float y1, float x2, float y2);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual geometrize::ShapeTypes getType() const override;

    float m_x1; ///< Left coordinate.
    float m_y1; ///< Top coordinate.
    float m_x2; ///< Right coordinate.
    float m_y2; ///< Bottom coordinate.
};

}
