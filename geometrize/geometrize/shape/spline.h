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
 * @brief The Spline class represents an open spline.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Spline : public Shape
{
public:
    /**
     * @brief Spline Creates a new spline.
     * @param model The model that created this shape.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Spline(const geometrize::Model& model, std::int32_t xBound, std::int32_t yBound);

    virtual std::shared_ptr<geometrize::Shape> clone() const;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

private:
    const geometrize::Model& m_model; ///< The model that produces the shape.

    const std::int32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::int32_t m_yBound; ///< The y-bound of the whole canvas.

    std::vector<std::pair<std::int32_t, std::int32_t>> m_controlPoints; ///< The control points of the spline.
};

}
