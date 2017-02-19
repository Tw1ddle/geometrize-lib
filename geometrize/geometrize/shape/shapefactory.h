#pragma once

#include <cstdint>
#include <memory>

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
std::shared_ptr<geometrize::Shape> create(geometrize::shapes::ShapeTypes t, std::uint32_t xBound, std::uint32_t yBound);

/**
 * @brief randomShape Creates a random shape.
 * @param xBound The x-bound of the whole canvas.
 * @param yBound The y-bound of the whole canvas.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> randomShape(std::uint32_t xBound, std::uint32_t yBound);

/**
 * @brief randomShapeOf Creates a random shape from the types supplied.
 * @param t The types of shape to possibly create.
 * @param xBound The x-bound of the whole canvas.
 * @param yBound The y-bound of the whole canvas.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> randomShapeOf(geometrize::shapes::ShapeTypes t, std::uint32_t xBound, std::uint32_t yBound);

}
