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
    virtual void translate(float x, float y) override;
    virtual void scale(float scaleFactor) override;
    virtual geometrize::ShapeTypes getType() const override;
    virtual std::vector<float> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

    float m_x; ///< x-coordinate.
    float m_y; ///< y-coordinate.
    float m_r; ///< Radius.
};

}
