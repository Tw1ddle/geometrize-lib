#pragma once

#include <memory>
#include <vector>

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
    ImageRunner(const geometrize::Bitmap& bitmap);
    ~ImageRunner();
    ImageRunner& operator=(const ImageRunner&) = delete;
    ImageRunner(const ImageRunner&) = delete;

    /**
     * @brief step Updates the internal model once.
     * @return A vector containing data about the shapes just added to the internal model.
     */
    std::vector<geometrize::ShapeResult> step();

    /**
     * @brief getCurrent Gets the current bitmap with the primitives drawn on it.
     * @return The current bitmap.
     */
    geometrize::Bitmap& getCurrent();

    /**
     * @brief getTarget Gets the target bitmap.
     * @return The target bitmap.
     */
    geometrize::Bitmap& getTarget();

private:
    class ImageRunnerImpl;
    std::unique_ptr<ImageRunner::ImageRunnerImpl> d;
};

}
