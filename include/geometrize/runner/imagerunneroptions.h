#pragma once

#include <cstdint>

#include "../shape/shapetypes.h"

namespace geometrize
{

/**
 * @brief The ImageRunnerOptions struct encapsulates the parameters that may be passed to the image runner.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct ImageRunnerOptions
{
    std::int32_t primitiveCount = 50; // TODO remove defaults?
    geometrize::shapes::ShapeTypes shapeTypes = geometrize::shapes::ShapeTypes::RECTANGLE;
};

}
