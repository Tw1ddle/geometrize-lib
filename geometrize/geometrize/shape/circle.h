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
 * @brief The Circle class represents a circle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Circle : public Shape
{
public:
    /**
     * @brief Circle Creates a new circle.
     * @param model The model that created this shape.
     */
    Circle(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData(const std::string& attribs) const override;

private:
    const geometrize::Model& m_model; ///< The model that produces the shape.

    std::int32_t m_x; ///< x-coordinate.
    std::int32_t m_y; ///< y-coordinate.
    std::int32_t m_r; ///< Radius.
};

}
