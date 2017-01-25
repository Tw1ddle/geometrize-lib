#pragma once

#include <vector>

#include "../bitmap/bitmapdata.h"
#include "../core.h"
#include "../model.h"
#include "../shape/shapetypes.h"
#include "imagerunneroptions.h"

namespace geometrize
{

/**
 * @brief The ImageRunner class is a helper class for creating a set of primitives from a single source image.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ImageRunner
{
public:
    inline ImageRunner(BitmapData& bitmap) : m_model{bitmap, geometrize::core::getAverageImageColor(bitmap)} {} // TODO set the starting color optionally
    inline ~ImageRunner() = default;
    inline ImageRunner& operator=(const ImageRunner&) = delete;
    inline ImageRunner(const ImageRunner&) = delete;

    /**
     * @brief Updates runner until the number of primitives specified in the runner options are added.
     * @return A vector containing data about the shapes added to the model in this step.
     */
    inline std::vector<ShapeResult> run()
    {
        std::vector<ShapeResult> results(m_options.primitiveCount);
        while(results.size() <= m_options.primitiveCount) {
            std::vector<ShapeResult> shapes{m_model.step(m_options.shapeTypes, 128, 0)}; // TODO alpha, repeat etc
            std::move(shapes.begin(), shapes.end(), std::back_inserter(results));
        }
        return results;
    }

    /**
     * @brief Updates the model once.
     * @return A vector containing data about the shapes just added to the model.
     */
    inline std::vector<ShapeResult> step()
    {
        return m_model.step(m_options.shapeTypes, 128, 0); // TODO alpha, repeat params from config etc
    }

    /**
     * @brief Gets the current bitmap with the primitives drawn on it.
     * @return The current bitmap.
     */
    inline BitmapData& getBitmapData()
    {
        return m_model.getCurrent();
    }

    /**
     * @brief Gets data about the shapes added to the model so far.
     * @return The shape results.
     */
    inline std::vector<ShapeResult>& getShapeResults()
    {
        return m_model.getShapeResults();
    }

private:
    Model m_model; ///< The model for the primitive optimization/fitting algorithm.
    ImageRunnerOptions m_options; ///< The runtime configuration parameters for the runner.
};

}
