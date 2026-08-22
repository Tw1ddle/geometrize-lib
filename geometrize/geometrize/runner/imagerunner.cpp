#include "imagerunner.h"

#include <functional>
#include <memory>
#include <vector>

#include "../bitmap/bitmap.h"
#include "../commonutil.h"
#include "../core.h"
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

    std::vector<geometrize::ShapeResult> step(const geometrize::ImageRunnerOptions& options,
                                              std::function<std::shared_ptr<geometrize::Shape>()> shapeCreator,
                                              geometrize::core::EnergyFunction energyFunction,
                                              geometrize::ShapeAcceptancePreconditionFunction addShapePrecondition)
    {
        const geometrize::Model& model{m_model};
        const auto [xMin, yMin, xMax, yMax] = geometrize::commonutil::mapShapeBoundsToImage(options.shapeBounds, model.getTarget());
        const geometrize::ShapeTypes types = options.shapeTypes;
        const bool usingDefaultShapeCreator{!shapeCreator};

        if(!shapeCreator) {
            shapeCreator = geometrize::createDefaultShapeCreator(types, xMin, yMin, xMax, yMax);
        }

        m_model.setSeed(options.seed);
        return m_model.stepWithGpuBatching(shapeCreator, options.alpha, options.shapeCount, options.maxShapeMutations, options.maxThreads, energyFunction, addShapePrecondition, usingDefaultShapeCreator);
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
        const geometrize::Model& model{m_model};
        return model.getCurrent();
    }

    const geometrize::Bitmap& getTarget() const
    {
        const geometrize::Model& model{m_model};
        return model.getTarget();
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

std::vector<geometrize::ShapeResult> ImageRunner::step(const geometrize::ImageRunnerOptions& options,
                                                       std::function<std::shared_ptr<geometrize::Shape>()> shapeCreator,
                                                       geometrize::core::EnergyFunction energyFunction,
                                                       geometrize::ShapeAcceptancePreconditionFunction addShapePrecondition)
{
    return d->step(options, shapeCreator, energyFunction, addShapePrecondition);
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
    const ImageRunner::ImageRunnerImpl& impl{*d};
    return impl.getCurrent();
}

const geometrize::Bitmap& ImageRunner::getTarget() const
{
    const ImageRunner::ImageRunnerImpl& impl{*d};
    return impl.getTarget();
}

geometrize::Model& ImageRunner::getModel()
{
    return d->getModel();
}

}
