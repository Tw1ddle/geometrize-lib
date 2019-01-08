#pragma once

#include <cstdint>
#include <memory>

#include "../model.h"
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
