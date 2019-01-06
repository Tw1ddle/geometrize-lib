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
 * @brief The Ellipse class represents an ellipse.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Ellipse : public Shape
{
public:
    Ellipse() = default;
    Ellipse(float x, float y, float rx, float ry);
    /**
     * @brief Ellipse Creates a new ellipse.
     * @param model The model that created this shape.
     */
    Ellipse(const geometrize::Model& model);

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
};

}
