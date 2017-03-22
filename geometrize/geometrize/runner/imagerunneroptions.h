#pragma once

#include <cstdint>

#include "../shape/shapetypes.h"

namespace geometrize
{

/**
 * @brief The ImageRunnerOptions class encapsulates the parameters that may be passed to the image runner.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ImageRunnerOptions
{
public:
    geometrize::ShapeTypes shapeTypes = geometrize::ShapeTypes::ELLIPSE; ///< The shape types that the image runner shall use.
    std::uint8_t alpha = 128U; ///< The alpha/opacity of the shapes (0-255).
    std::uint32_t shapeCount = 200U; ///< The number of candidate shapes that will be tried per model step.
    std::uint32_t maxShapeMutations = 50U; ///< The number of times each candidate shape will be modified to attempt to find a better fit.
};

}
