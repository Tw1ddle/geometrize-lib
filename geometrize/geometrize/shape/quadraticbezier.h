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
 * @brief The QuadraticBezier class represents a quadratic bezier curve.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class QuadraticBezier : public Shape
{
public:
    /**
     * @brief QuadraticBezier Creates a new quadratic bezier curve.
     * @param model The model that created this shape.
     */
    QuadraticBezier(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

private:
    const geometrize::Model& m_model; ///< The model that produces the shape.

    std::vector<std::pair<std::int32_t, std::int32_t>> m_controlPoints; ///< The control points of the quadratic bezier.
};

}
