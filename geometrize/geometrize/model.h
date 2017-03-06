#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "shaperesult.h"
#include "shape/shapetypes.h"

namespace geometrize
{
class Bitmap;
class Shape;
}

namespace geometrize
{

/**
 * @brief The Model class is the model for the core optimization/fitting algorithm.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Model
{
public:
    /**
     * @brief Model Creates a model that will aim to replicate the target bitmap with shapes, starting from a bitmap with the given background color.
     * @param target The target bitmap to replicate with shapes.
     * @param backgroundColor The background color of the starting bitmap.
     */
    Model(const geometrize::Bitmap& target, geometrize::rgba backgroundColor);

    /**
     * @brief Model Creates a model that will optimize for the given target bitmap, starting from the given initial bitmap.
     * The target bitmap and initial bitmap must be the same size (width and height).
     * @param target The target bitmap to replicate with shapes.
     * @param initial The starting bitmap.
     */
    Model(const geometrize::Bitmap& target, const geometrize::Bitmap& initial);
    ~Model();
    Model& operator=(const Model&) = delete;
    Model(const Model&) = delete;

    /**
     * @brief reset Resets the model back to the state it was in when it was created.
     * @param backgroundColor The starting background color to use.
     */
    void reset(geometrize::rgba backgroundColor);

    /**
     * @brief getWidth Gets the width of the target bitmap.
     * @return The width of the target bitmap.
     */
    std::int32_t getWidth() const;

    /**
     * @brief getHeight Gets the height of the target bitmap.
     * @return The height of the target bitmap.
     */
    std::int32_t getHeight() const;

    /**
     * @brief step Steps the primitive optimization/fitting algorithm.
     * @param shapeTypes The types of shape to use.
     * @param alpha The alpha of the shape.
     * @param shapeCount The number of random shapes to generate (only 1 is chosen in the end).
     * @param maxShapeMutations The maximum number of times to mutate each random shape.
     * @return A vector containing data about the shapes added to the model in this step.
     */
    std::vector<geometrize::ShapeResult> step(
            geometrize::shapes::ShapeTypes shapeTypes,
            std::uint8_t alpha,
            std::uint32_t shapeCount,
            std::uint32_t maxShapeMutations);

    /**
     * @brief drawShape Draws a shape on the model. The appropriate color to use is determined by the model.
     * @param shape The shape to draw.
     * @param alpha The alpha/opacity of the shape.
     * @return Data about the shape drawn on the model.
     */
    geometrize::ShapeResult drawShape(std::shared_ptr<geometrize::Shape> shape, std::uint8_t alpha);

    /**
     * @brief drawShape Draws a shape on the model.
     * @param shape The shape to draw.
     * @param color The color (including alpha) of the shape.
     * @return Data about the shape drawn on the model.
     */
    geometrize::ShapeResult drawShape(std::shared_ptr<geometrize::Shape> shape, geometrize::rgba color);

    /**
     * @brief getCurrent Gets the current bitmap.
     * @return The current bitmap.
     */
    geometrize::Bitmap& getCurrent();

    /**
     * @brief getTarget Gets the target bitmap.
     * @return The target bitmap.
     */
    geometrize::Bitmap& getTarget();

    /**
     * @brief setMaxThreads Sets the maximum number of threads the model will use when stepping.
     * @param threadCount The maximum number of threads to use.
     */
    void setMaxThreads(std::uint32_t threadCount);

private:
    class ModelImpl;
    std::unique_ptr<Model::ModelImpl> d;
};

}
