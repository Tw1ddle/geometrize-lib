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
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct ShapeResult
{
    const float score;
    const geometrize::rgba color;
    const std::shared_ptr<geometrize::Shape> shape;
};

}
