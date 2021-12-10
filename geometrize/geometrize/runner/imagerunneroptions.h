#pragma once

#include <cstdint>

#include "../shape/shapetypes.h"

namespace geometrize
{

/**
 * @brief The ImageRunnerOptionsShapeBounds struct encapsulates options for where shapes may be drawn within the image.
 * @author Sam Twidale (https://samcodes.co.uk/)
 */
struct ImageRunnerShapeBoundsOptions {
    bool enabled = false; // Whether to use these bounds, or to use the bounds of the target image instead (these can't be larger than the image in any case)
    std::uint32_t xMin = 0;
    std::uint32_t yMin = 0;
    std::uint32_t xMax = 0;
    std::uint32_t yMax = 0;
};

/**
 * @brief The ImageRunnerOptions class encapsulates preferences/options that the image runner uses.
 * @author Sam Twidale (https://samcodes.co.uk/)
 */
class ImageRunnerOptions
{
public:
    geometrize::ShapeTypes shapeTypes = geometrize::ShapeTypes::ELLIPSE; ///< The shape types that the image runner shall use.
    std::uint8_t alpha = 128U; ///< The alpha/opacity of the shapes (0-255).
    std::uint32_t shapeCount = 50U; ///< The number of candidate shapes that will be tried per model step.
    std::uint32_t maxShapeMutations = 100U; ///< The maximum number of times each candidate shape will be modified to attempt to find a better fit.
    std::uint32_t seed = 9001U; ///< The seed for the random number generators used by the image runner.
    std::uint32_t maxThreads = 0; ///< The maximum number of separate threads for the implementation to use. 0 lets the implementation choose a reasonable number.
    ImageRunnerShapeBoundsOptions shapeBounds{}; ///< The bounds within which shapes may be drawn. If left unspecified, the bounds of the target image are used i.e. (0, 0, imageWidth, imageHeight)
};

}
