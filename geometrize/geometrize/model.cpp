#include "model.h"

#include <cstdint>
#include <vector>

#include "bitmap/bitmap.h"
#include "core.h"
#include "shape/shape.h"
#include "shaperesult.h"
#include "shape/shapetypes.h"

namespace geometrize
{

Model::Model(const geometrize::Bitmap& target, const geometrize::rgba backgroundColor) :
    m_target(target),
    m_current(target.getWidth(), target.getHeight(), backgroundColor),
    m_buffer(target.getWidth(), target.getHeight(), backgroundColor),
    m_score{geometrize::core::differenceFull(m_target, m_current)}
{}

void Model::reset(const geometrize::rgba backgroundColor)
{
    m_current.fill(backgroundColor);
    m_buffer.fill(backgroundColor);
    m_score = geometrize::core::differenceFull(m_target, m_current);
}

std::uint32_t Model::getWidth() const
{
    return m_target.getWidth();
}

std::uint32_t Model::getHeight() const
{
    return m_target.getHeight();
}

float Model::getAspectRatio() const
{
    if(m_target.getWidth() == 0 || m_target.getHeight() == 0) {
        return 0;
    }
    return static_cast<float>(m_target.getWidth()) / static_cast<float>(m_target.getHeight());
}

std::vector<geometrize::ShapeResult> Model::step(const geometrize::shapes::ShapeTypes shapeTypes, const std::uint8_t alpha, const std::uint32_t repeats)
{
    geometrize::State state{geometrize::core::bestHillClimbState(shapeTypes, alpha, 1000, 100, 16, m_target, m_current, m_buffer)}; // TODO pass more params
    std::vector<geometrize::ShapeResult> results;
    results.push_back(addShape(state.m_shape, alpha));

    for(std::uint32_t i = 0; i < repeats; i++) {
        const float before{state.calculateEnergy(m_target, m_current, m_buffer)};
        state = geometrize::core::hillClimb(state, 100, m_target, m_current, m_buffer);
        const float after{state.calculateEnergy(m_target, m_current, m_buffer)};
        if(before == after) {
            break;
        }
        results.push_back(addShape(state.m_shape, state.m_alpha));
    }
    return results;
}

geometrize::ShapeResult Model::addShape(geometrize::Shape* shape, const std::uint8_t alpha)
{
    const geometrize::Bitmap before{m_current};
    const std::vector<geometrize::Scanline> lines{shape->rasterize()};
    const geometrize::rgba color{geometrize::core::computeColor(m_target, m_current, lines, alpha)};
    geometrize::core::drawLines(m_current, color, lines);

    m_score = geometrize::core::differencePartial(m_target, before, m_current, m_score, lines);

    geometrize::ShapeResult result{m_score, color, shape};
    return result;
}

geometrize::Bitmap& Model::getCurrent()
{
    return m_current;
}

}
