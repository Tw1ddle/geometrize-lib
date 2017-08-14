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
    /**
     * @brief Ellipse Creates a new ellipse.
     * @param model The model that created this shape.
     */
    Ellipse(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

    std::int32_t m_x; ///< x-coordinate.
    std::int32_t m_y; ///< y-coordinate.
    std::int32_t m_rx; ///< x-radius.
    std::int32_t m_ry; ///< y-radius.
};

}
