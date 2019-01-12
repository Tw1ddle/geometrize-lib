#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "../rasterizer/scanline.h"
#include "shapetypes.h"

namespace geometrize
{

/**
 * Base class for shape rasterization and manipulation.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Shape
{
public:
    Shape() = default;
    virtual ~Shape() = default;
    Shape& operator=(const geometrize::Shape& other) = default;
    Shape(const geometrize::Shape& other) = default;

    std::function<void(geometrize::Shape&)> setup;
    std::function<void(geometrize::Shape&)> mutate;
    std::function<std::vector<geometrize::Scanline>(const geometrize::Shape&)> rasterize;

    /**
     * @brief clone Clones the shape, a virtual copy constructor.
     * @return A clone of the shape.
     */
    virtual std::shared_ptr<geometrize::Shape> clone() const = 0;

    /**
     * @brief getType Gets the ShapeType of the shape.
     * @return The ShapeType of the shape.
     */
    virtual geometrize::ShapeTypes getType() const = 0;
};

}
