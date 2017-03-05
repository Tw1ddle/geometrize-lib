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
 * @brief The Triangle class represents a triangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Triangle : public Shape
{
public:
    /**
     * @brief Triangle Creates a new triangle.
     * @param model The model that created this shape.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Triangle(const geometrize::Model& model, std::int32_t xBound, std::int32_t yBound);

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

    std::int32_t m_x1; ///< First x-coordinate.
    std::int32_t m_y1; ///< First y-coordinate.
    std::int32_t m_x2; ///< Second x-coordinate.
    std::int32_t m_y2; ///< Second y-coordinate.
    std::int32_t m_x3; ///< Third x-coordinate.
    std::int32_t m_y3; ///< Third y-coordinate.
};

}
