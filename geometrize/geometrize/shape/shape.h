#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../rasterizer/scanline.h"
#include "shapetypes.h"

namespace geometrize
{
class Model;
}

namespace geometrize
{

/**
 * Base class for shape rasterization and manipulation.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Shape
{
public:
    Shape(const geometrize::Model& model);
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
     * @brief mutate Modifies the shape a little, typically using a random component.
     * Used to improve the shape's fit in a bitmap.
     */
    virtual void mutate() = 0;

    /**
     * @brief getType Gets the ShapeType of the shape.
     * @return The ShapeType of the shape.
     */
    virtual geometrize::ShapeTypes getType() const = 0;

    /**
     * @brief getRawShapeData Gets a vector of data that represents the shape geometry, the format varies depending on the ShapeType.
     * @return The shape data.
     */
    virtual std::vector<std::int32_t> getRawShapeData() const = 0;

    /**
     * @brief getSvgShapeData Gets a string that represents a SVG element that describes the shape geometry.
     * @return The SVG shape data that represents this shape.
     */
    virtual std::string getSvgShapeData() const = 0;

    /**
     * @brief SVG_STYLE_HOOK A hook that an SVG exporter should use to augment shape styling produced by the getSvgShapeData method.
     * Note that shape subclasses should include this in shape data - so an SVG exporter implementation must remove/replace this hook string to produce correct SVG files.
     */
    static const std::string SVG_STYLE_HOOK;

    /**
     * @brief getModel Gets the model that created this shape.
     * @return The model.
     */
    const Model& getModel();

    const geometrize::Model& m_model; ///< The model that creates, sets up and mutates shapes
};

}
