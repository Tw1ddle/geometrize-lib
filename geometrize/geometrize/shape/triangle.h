#pragma once

#include <cstdint>
#include <memory>

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
    Triangle() = default;
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual geometrize::ShapeTypes getType() const override;

    float m_x1; ///< First x-coordinate.
    float m_y1; ///< First y-coordinate.
    float m_x2; ///< Second x-coordinate.
    float m_y2; ///< Second y-coordinate.
    float m_x3; ///< Third x-coordinate.
    float m_y3; ///< Third y-coordinate.
};

}
