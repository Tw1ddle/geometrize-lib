#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "../shaperesult.h"

namespace geometrize
{
class Bitmap;
class ImageRunnerOptions;
class Model;
class Shape;
}

namespace geometrize
{

/**
 * @brief The ImageRunner class is a helper class for creating a set of primitives from a source image.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ImageRunner
{
public:
    /**
     * @brief ImageRunner Creates an new image runner with the given target bitmap. Uses the average color of the target as the starting image.
     * @param targetBitmap The target bitmap to replicate with shapes.
     */
    ImageRunner(const geometrize::Bitmap& targetBitmap);

    /**
     * @brief ImageRunner Creates an image runner with the given target bitmap, starting from the given initial bitmap.
     * The target bitmap and initial bitmap must be the same size (width and height).
     * @param targetBitmap The target bitmap to replicate with shapes.
     * @param initialBitmap The starting bitmap.
     */
    ImageRunner(const geometrize::Bitmap& targetBitmap, const geometrize::Bitmap& initialBitmap);
    ~ImageRunner();
    ImageRunner& operator=(const ImageRunner&) = delete;
    ImageRunner(const ImageRunner&) = delete;

    /**
     * @brief step Updates the internal model once.
     * @param options Various configurable settings for doing the step e.g. the shape types to consider.
     * @return A vector containing data about the shapes just added to the internal model.
     */
    std::vector<geometrize::ShapeResult> step(const geometrize::ImageRunnerOptions& options, std::function<std::shared_ptr<geometrize::Shape>()> shapeCreator = nullptr);

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

    /**
     * @brief getCurrent Gets the current bitmap with the primitives drawn on it, const-edition.
     * @return The current bitmap.
     */
    const geometrize::Bitmap& getCurrent() const;

    /**
     * @brief getTarget Gets the target bitmap, const-edition.
     * @return The target bitmap.
     */
    const geometrize::Bitmap& getTarget() const;

    /**
     * @brief getModel Gets the underlying model.
     * @return The model.
     */
    geometrize::Model& getModel();

private:
    class ImageRunnerImpl;
    std::unique_ptr<ImageRunner::ImageRunnerImpl> d;
};

}
