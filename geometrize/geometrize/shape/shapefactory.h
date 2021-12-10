#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include "shape.h"
#include "shapetypes.h"

namespace geometrize
{
class Shape;
}

namespace geometrize
{

/**
 * @brief createDefaultShapeCreator Creates an instance of the default shape creator object.
 * The setup, mutate and rasterize methods are bound with default methods.
 * @param types The types of shapes to create.
 * @param xMin The minimum x coordinate of the shapes created.
 * @param yMin The minimum y coordinate of the shapes created.
 * @param xMax The maximum x coordinate of the shapes created.
 * @param yMax The maximum y coordinate of the shapes created.
 * @return The default shape creator.
 */
std::function<std::shared_ptr<geometrize::Shape>()> createDefaultShapeCreator(geometrize::ShapeTypes types, std::int32_t xMin, std::int32_t yMin, std::int32_t xMax, std::int32_t yMax);

/**
 * @brief create Creates a new shape of the specified type.
 * @param t The type of shape to create.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> create(geometrize::ShapeTypes t);

/**
 * @brief randomShape Creates a random shape.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> randomShape();

/**
 * @brief randomShapeOf Creates a random shape from the types supplied.
 * @param t The types of shape to possibly create.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> randomShapeOf(geometrize::ShapeTypes t);

}
