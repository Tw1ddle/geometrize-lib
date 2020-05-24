#pragma once

#include <memory>

#include "bitmap/rgba.h"

namespace geometrize
{
class Shape;
}

namespace geometrize
{

/**
 * @brief The ShapeResult struct is a container for info about a shape added to the model.
 * @author Sam Twidale (https://samcodes.co.uk/)
 */
struct ShapeResult
{
    const double score;
    const geometrize::rgba color;
    const std::shared_ptr<geometrize::Shape> shape;
};

}
