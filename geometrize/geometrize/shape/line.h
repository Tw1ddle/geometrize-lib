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
 * @brief The Line class represents a simple line.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Line : public Shape
{
public:
    Line() = default;
    Line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2);
    /**
     * @brief Line Creates a new line.
     * @param model The model that created this shape.
     */
    Line(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

    std::int32_t m_x1; ///< First x-coordinate.
    std::int32_t m_y1; ///< First y-coordinate.
    std::int32_t m_x2; ///< Second x-coordinate.
    std::int32_t m_y2; ///< Second y-coordinate.
};

}
