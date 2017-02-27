#include "imagerunner.h"

#include <memory>
#include <vector>

#include "../bitmap/bitmap.h"
#include "../core.h"
#include "../model.h"
#include "../shape/shapetypes.h"
#include "imagerunneroptions.h"

namespace geometrize
{

class ImageRunner::ImageRunnerImpl
{
public:
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap) : m_model{targetBitmap, geometrize::core::getAverageImageColor(targetBitmap)} {}
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap, const geometrize::Bitmap& initialBitmap) : m_model{targetBitmap, initialBitmap} {}
    ~ImageRunnerImpl() = default;
    ImageRunnerImpl& operator=(const ImageRunnerImpl&) = delete;
    ImageRunnerImpl(const ImageRunnerImpl&) = delete;

    std::vector<geometrize::ShapeResult> step()
    {
        return m_model.step(m_options.shapeTypes, 128); // TODO alpha, repeat params from config etc
    }

    geometrize::Bitmap& getCurrent()
    {
        return m_model.getCurrent();
    }

    geometrize::Bitmap& getTarget()
    {
        return m_model.getTarget();
    }

private:
    geometrize::Model m_model; ///< The model for the primitive optimization/fitting algorithm.
    geometrize::ImageRunnerOptions m_options; ///< The runtime configuration parameters for the runner.
};

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap) :
    d{std::make_unique<ImageRunner::ImageRunnerImpl>(targetBitmap)}
{}

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap,  const geometrize::Bitmap& initialBitmap) :
    d{std::make_unique<ImageRunner::ImageRunnerImpl>(targetBitmap, initialBitmap)}
{}

ImageRunner::~ImageRunner()
{}

std::vector<geometrize::ShapeResult> ImageRunner::step()
{
    return d->step();
}

geometrize::Bitmap& ImageRunner::getCurrent()
{
    return d->getCurrent();
}

geometrize::Bitmap& ImageRunner::getTarget()
{
    return d->getTarget();
}

}
