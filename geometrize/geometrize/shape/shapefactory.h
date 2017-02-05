#pragma once

#include <cstdint>

#include "shapetypes.h"

namespace geometrize
{
class Shape;
}

namespace geometrize
{

/**
 * @brief create Creates a new shape of the specified type.
 * @param t The type of shape to create.
 * @param xBound The x-bound of the whole canvas.
 * @param yBound The y-bound of the whole canvas.
 * @return The new shape.
 */
geometrize::Shape* create(geometrize::shapes::ShapeTypes t, const std::uint32_t xBound, const std::uint32_t yBound);

/**
 * @brief randomShape Creates a random shape.
 * @param xBound The x-bound of the whole canvas.
 * @param yBound The y-bound of the whole canvas.
 * @return The new shape.
 */
geometrize::Shape* randomShape(const std::uint32_t xBound, const std::uint32_t yBound);

/**
 * @brief randomShapeOf Creates a random shape from the types supplied.
 * @param t The types of shape to possibly create.
 * @param xBound The x-bound of the whole canvas.
 * @param yBound The y-bound of the whole canvas.
 * @return The new shape.
 */
geometrize::Shape* randomShapeOf(const geometrize::shapes::ShapeTypes t, const std::uint32_t xBound, const std::uint32_t yBound);

}
