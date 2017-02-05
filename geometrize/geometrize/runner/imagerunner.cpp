#include "imagerunner.h"

#include <vector>

#include "../bitmap/bitmap.h"
#include "../core.h"
#include "../model.h"
#include "../shape/shapetypes.h"
#include "imagerunneroptions.h"

namespace geometrize
{

ImageRunner::ImageRunner(geometrize::Bitmap& bitmap) : m_model{bitmap, geometrize::core::getAverageImageColor(bitmap)} {} // TODO set the starting color optionally

std::vector<geometrize::ShapeResult> ImageRunner::step()
{
    return m_model.step(m_options.shapeTypes, 128, 0); // TODO alpha, repeat params from config etc
}

geometrize::Bitmap& ImageRunner::getBitmap()
{
    return m_model.getCurrent();
}

}
