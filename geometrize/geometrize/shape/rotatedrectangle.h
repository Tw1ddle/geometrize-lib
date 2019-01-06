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
 * @brief The RotatedRectangle class represents a rotated rectangle.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class RotatedRectangle : public Shape
{
public:
    RotatedRectangle() = default;
    RotatedRectangle(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, std::int32_t angle);
    /**
     * @brief RotatedRectangle Creates a new rotated rectangle.
     * @param model The model that created this shape.
     */
    RotatedRectangle(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

    std::vector<std::pair<std::int32_t, std::int32_t>> getCornerPoints() const;

    std::int32_t m_x1; ///< Left coordinate.
    std::int32_t m_y1; ///< Top coordinate.
    std::int32_t m_x2; ///< Right coordinate.
    std::int32_t m_y2; ///< Bottom coordinate.
    std::int32_t m_angle; ///< Rotation angle.
};

}
