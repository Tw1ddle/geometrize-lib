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
    ImageRunnerImpl(geometrize::Bitmap& bitmap) : m_model{bitmap, geometrize::core::getAverageImageColor(bitmap)} {} // TODO set the starting color optionally
    ~ImageRunnerImpl() = default;
    ImageRunnerImpl& operator=(const ImageRunnerImpl&) = delete;
    ImageRunnerImpl(const ImageRunnerImpl&) = delete;

    std::vector<geometrize::ShapeResult> step()
    {
        return m_model.step(m_options.shapeTypes, 128, 0); // TODO alpha, repeat params from config etc
    }

    geometrize::Bitmap& getBitmap()
    {
        return m_model.getCurrent();
    }

private:
    geometrize::Model m_model; ///< The model for the primitive optimization/fitting algorithm.
    geometrize::ImageRunnerOptions m_options; ///< The runtime configuration parameters for the runner.
};

ImageRunner::ImageRunner(geometrize::Bitmap& bitmap) : d{std::make_unique<ImageRunner::ImageRunnerImpl>(bitmap)}
{}

ImageRunner::~ImageRunner()
{}

std::vector<geometrize::ShapeResult> ImageRunner::step()
{
    return d->step();
}

geometrize::Bitmap& ImageRunner::getBitmap()
{
    return d->getBitmap();
}

}
