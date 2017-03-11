#pragma once

#include <memory>
#include <string>
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
    Shape& operator=(const geometrize::Shape& other) = default;
    Shape(const geometrize::Shape& other) = default;

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
     * @brief getRawShapeData Gets a vector of data that represents the shape geometry, the format varies depending on the ShapeType.
     * @return The shape data.
     */
    virtual std::vector<std::int32_t> getRawShapeData() const = 0;

    /**
     * @brief getSvgShapeData Gets a string that represents a SVG element that describes the shape geometry.
     * @param attribs The attributes (such as a style attribute) to include in the SVG shape data.
     * @return The SVG shape data that represents this shape.
     */
    virtual std::string getSvgShapeData(const std::string& attribs) const = 0;
};

}
