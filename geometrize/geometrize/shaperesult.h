#pragma once

#include "bitmap/rgba.h"
#include "shape/shape.h"

namespace geometrize
{

/**
 * @brief The ShapeResult struct is a container for info about a shape added to the model.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct ShapeResult
{
    float score;
    geometrize::rgba color;
    std::shared_ptr<geometrize::Shape> shape;
};

}
