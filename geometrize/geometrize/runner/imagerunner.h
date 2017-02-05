#pragma once

#include <vector>

#include "imagerunneroptions.h"
#include "../model.h"
#include "../shaperesult.h"

namespace geometrize
{
class Bitmap;
}

namespace geometrize
{

/**
 * @brief The ImageRunner class is a helper class for creating a set of primitives from a single source image.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ImageRunner
{
public:
    ImageRunner(geometrize::Bitmap& bitmap);
    ~ImageRunner() = default;
    ImageRunner& operator=(const ImageRunner&) = delete;
    ImageRunner(const ImageRunner&) = delete;

    /**
     * @brief Updates runner until the number of primitives specified in the runner options are added.
     * @return A vector containing data about the shapes added to the model in this step.
     */
    std::vector<geometrize::ShapeResult> run();

    /**
     * @brief Updates the model once.
     * @return A vector containing data about the shapes just added to the model.
     */
    std::vector<geometrize::ShapeResult> step();

    /**
     * @brief Gets the current bitmap with the primitives drawn on it.
     * @return The current bitmap.
     */
    geometrize::Bitmap& getBitmap();

private:
    geometrize::Model m_model; ///< The model for the primitive optimization/fitting algorithm.
    geometrize::ImageRunnerOptions m_options; ///< The runtime configuration parameters for the runner.
};

}
