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

std::vector<geometrize::ShapeResult> ImageRunner::run()
{
    std::vector<geometrize::ShapeResult> results(m_options.primitiveCount);
    while(results.size() <= m_options.primitiveCount) {
        std::vector<geometrize::ShapeResult> shapes{m_model.step(m_options.shapeTypes, 128, 0)}; // TODO alpha, repeat etc
        std::move(shapes.begin(), shapes.end(), std::back_inserter(results));
    }
    return results;
}

std::vector<geometrize::ShapeResult> ImageRunner::step()
{
    return m_model.step(m_options.shapeTypes, 128, 0); // TODO alpha, repeat params from config etc
}

geometrize::Bitmap& ImageRunner::getBitmap()
{
    return m_model.getCurrent();
}

}
