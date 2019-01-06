#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{
class Model;
}

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
    /**
     * @brief RotatedEllipse Creates a new rotated ellipse.
     * @param model The model that created this shape.
     */
    RotatedEllipse(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual void translate(float x, float y) override;
    virtual void scale(float scaleFactor) override;
    virtual geometrize::ShapeTypes getType() const override;
    virtual std::vector<float> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

    float m_x; ///< x-coordinate.
    float m_y; ///< y-coordinate.
    float m_rx; ///< x-radius.
    float m_ry; ///< y-radius.
    float m_angle; ///< Rotation angle.
};

}
