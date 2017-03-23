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
 * @param model The model to create the shape with.
 * @param t The type of shape to create.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> create(const geometrize::Model& model, geometrize::ShapeTypes t);

/**
 * @brief randomShape Creates a random shape.
 * @param model The model to create the shape with.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> randomShape(const geometrize::Model& model);

/**
 * @brief randomShapeOf Creates a random shape from the types supplied.
 * @param model The model to create the shape with.
 * @param t The types of shape to possibly create.
 * @return The new shape.
 */
std::shared_ptr<geometrize::Shape> randomShapeOf(const geometrize::Model& model, geometrize::ShapeTypes t);

}
