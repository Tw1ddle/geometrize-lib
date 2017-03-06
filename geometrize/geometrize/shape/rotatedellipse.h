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
    /**
     * @brief RotatedEllipse Creates a new rotated ellipse.
     * @param model The model that created this shape.
     */
    RotatedEllipse(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

private:
    const geometrize::Model& m_model; ///< The model that produces the shape.

    std::int32_t m_x; ///< x-coordinate.
    std::int32_t m_y; ///< y-coordinate.
    std::int32_t m_rx; ///< x-radius.
    std::int32_t m_ry; ///< y-radius.
    std::int32_t m_angle; ///< Rotation angle.
};

}
