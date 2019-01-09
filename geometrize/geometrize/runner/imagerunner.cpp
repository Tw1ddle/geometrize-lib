#include "imagerunner.h"

#include <functional>
#include <memory>
#include <vector>

#include "../bitmap/bitmap.h"
#include "../model.h"
#include "../shape/shape.h"
#include "../shape/shapefactory.h"
#include "../shape/shapetypes.h"
#include "imagerunneroptions.h"

namespace geometrize
{

class ImageRunner::ImageRunnerImpl
{
public:
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap) : m_model{targetBitmap} {}
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap, const geometrize::Bitmap& initialBitmap) : m_model{targetBitmap, initialBitmap} {}
    ~ImageRunnerImpl() = default;
    ImageRunnerImpl& operator=(const ImageRunnerImpl&) = delete;
    ImageRunnerImpl(const ImageRunnerImpl&) = delete;

    std::vector<geometrize::ShapeResult> step(const geometrize::ImageRunnerOptions& options, std::function<std::shared_ptr<geometrize::Shape>()> shapeCreator)
    {
        const std::int32_t w = m_model.getTarget().getWidth();
        const std::int32_t h = m_model.getTarget().getHeight();
        const geometrize::ShapeTypes types = options.shapeTypes;

        if(!shapeCreator) {
            shapeCreator = geometrize::createDefaultShapeCreator(types, w, h);
        }

        m_model.setSeed(options.seed);
        return m_model.step(shapeCreator, options.alpha, options.shapeCount, options.maxShapeMutations, options.maxThreads);
    }

    geometrize::Bitmap& getCurrent()
    {
        return m_model.getCurrent();
    }

    geometrize::Bitmap& getTarget()
    {
        return m_model.getTarget();
    }

    const geometrize::Bitmap& getCurrent() const
    {
        return m_model.getCurrent();
    }

    const geometrize::Bitmap& getTarget() const
    {
        return m_model.getTarget();
    }

    geometrize::Model& getModel()
    {
        return m_model;
    }

private:
    geometrize::Model m_model; ///< The model for the primitive optimization/fitting algorithm.
};

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap) :
    d{std::unique_ptr<ImageRunner::ImageRunnerImpl>(new ImageRunner::ImageRunnerImpl(targetBitmap))}
{}

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap,  const geometrize::Bitmap& initialBitmap) :
    d{std::unique_ptr<ImageRunner::ImageRunnerImpl>(new ImageRunner::ImageRunnerImpl(targetBitmap, initialBitmap))}
{}

ImageRunner::~ImageRunner()
{}

std::vector<geometrize::ShapeResult> ImageRunner::step(const geometrize::ImageRunnerOptions& options, std::function<std::shared_ptr<geometrize::Shape>()> shapeCreator)
{
    return d->step(options, shapeCreator);
}

geometrize::Bitmap& ImageRunner::getCurrent()
{
    return d->getCurrent();
}

geometrize::Bitmap& ImageRunner::getTarget()
{
    return d->getTarget();
}

const geometrize::Bitmap& ImageRunner::getCurrent() const
{
    return d->getCurrent();
}

const geometrize::Bitmap& ImageRunner::getTarget() const
{
    return d->getTarget();
}

geometrize::Model& ImageRunner::getModel()
{
    return d->getModel();
}

}
