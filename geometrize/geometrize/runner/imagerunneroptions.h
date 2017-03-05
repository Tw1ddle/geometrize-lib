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
    geometrize::shapes::ShapeTypes shapeTypes = geometrize::shapes::ShapeTypes::ELLIPSE;
    std::uint8_t alpha = 128U;
    std::uint32_t shapeCount = 100U;
    std::uint32_t maxShapeMutations = 20U;
    std::uint32_t passes = 1U;
};

}
