#pragma once

#include <memory>
#include <vector>

#include "../scanline.h"
#include "shapetypes.h"

namespace geometrize
{

/**
 * Interface for shape rasterization and manipulation.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Shape
{
public:
    Shape() = default;
     ~Shape() = default;
    Shape& operator=(const Shape& other) = default;
    Shape(const Shape& other) = default;

    /**
     * @brief clone Clones the shape, a virtual copy constructor.
     * @return A clone of the shape.
     */
    virtual std::shared_ptr<geometrize::Shape> clone() const = 0;

    /**
     * @brief rasterize Creates a raster scanline representation of the shape.
     * @return Raster scanlines representing the shape.
     */
    virtual std::vector<geometrize::Scanline> rasterize() const = 0;

    /**
     * @brief mutate Modifies the shape a little, typically with a random component.
     * Used to improve the shape's fit in a bitmap (trial-and-error style).
     */
    virtual void mutate() = 0;

    /**
     * @brief getType Gets the ShapeType of the shape.
     * @return The ShapeType of the shape.
     */
    virtual geometrize::shapes::ShapeTypes getType() const = 0;

    /**
     * @brief getShapeData Gets a vector of data that represents the shape geometry, varies depending on the ShapeType.
     * @return The shape data.
     */
    virtual std::vector<std::int32_t> getShapeData() const = 0;
};

}
