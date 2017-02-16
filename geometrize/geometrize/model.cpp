#include "model.h"

#include <algorithm>
#include <cstdint>
#include <future>
#include <memory>
#include <vector>

#include "bitmap/bitmap.h"
#include "core.h"
#include "shape/shape.h"
#include "shaperesult.h"
#include "shape/shapetypes.h"

namespace geometrize
{

class Model::ModelImpl
{
public:
    ModelImpl(const geometrize::Bitmap& target, const geometrize::rgba backgroundColor) :
        m_target(target),
        m_current(target.getWidth(), target.getHeight(), backgroundColor),
        m_score{geometrize::core::differenceFull(m_target, m_current)}
    {}

    ~ModelImpl() = default;
    ModelImpl& operator=(const ModelImpl&) = delete;
    ModelImpl(const ModelImpl&) = delete;

    void reset(const geometrize::rgba backgroundColor)
    {
        m_current.fill(backgroundColor);
        m_score = geometrize::core::differenceFull(m_target, m_current);
    }

    std::uint32_t getWidth() const
    {
        return m_target.getWidth();
    }

    std::uint32_t getHeight() const
    {
        return m_target.getHeight();
    }

    float getAspectRatio() const
    {
        if(m_target.getWidth() == 0 || m_target.getHeight() == 0) {
            return 0;
        }
        return static_cast<float>(m_target.getWidth()) / static_cast<float>(m_target.getHeight());
    }

    std::vector<geometrize::State> getHillClimbState(const geometrize::shapes::ShapeTypes shapeTypes, const std::uint8_t alpha)
    {
        std::vector<std::future<geometrize::State>> futures;

        const std::uint32_t maxThreads{std::max(1U, std::thread::hardware_concurrency())};
        for(std::uint32_t i = 0; i < maxThreads; i++) {
            std::future<geometrize::State> handle{std::async(std::launch::async, [&]() {
                geometrize::Bitmap buffer{m_current};
                return state;
            })};
            futures.push_back(std::move(handle));
        }

        std::vector<geometrize::State> states;
        for(auto& f : futures) {
            states.push_back(f.get());
        }
        return states;
    }

    std::vector<geometrize::ShapeResult> step(const geometrize::shapes::ShapeTypes shapeTypes, const std::uint8_t alpha, const std::uint32_t repeats)
    {
        std::vector<geometrize::State> states{getHillClimbState(shapeTypes, alpha)};
        std::vector<geometrize::State>::iterator it = std::min_element(states.begin(), states.end(), [](const geometrize::State& a, const geometrize::State& b) {
            return a.m_score < b.m_score;
        });

        std::vector<geometrize::ShapeResult> results;
        results.push_back(drawShape((*it).m_shape, alpha));

        /*
        for(std::uint32_t i = 0; i < repeats; i++) {
            const float before{state.calculateEnergy(m_target, m_current, buffer)};
            state = geometrize::core::hillClimb(state, 100, m_target, m_current, buffer);
            const float after{state.calculateEnergy(m_target, m_current, buffer)};
            if(before == after) {
                break;
            }
            results.push_back(drawShape(state.m_shape, state.m_alpha));
        }
        */

        return results;
    }

    geometrize::ShapeResult drawShape(std::shared_ptr<geometrize::Shape> shape, const std::uint8_t alpha)
    {
        const geometrize::Bitmap before{m_current};
        const std::vector<geometrize::Scanline> lines{shape->rasterize()};
        const geometrize::rgba color{geometrize::core::computeColor(m_target, m_current, lines, alpha)};
        geometrize::core::drawLines(m_current, color, lines);

        m_score = geometrize::core::differencePartial(m_target, before, m_current, m_score, lines);

        geometrize::ShapeResult result{m_score, color, shape};
        return result;
    }

    geometrize::Bitmap& getCurrent()
    {
        return m_current;
    }

private:
    geometrize::Bitmap m_target; ///< The target bitmap, the bitmap we aim to approximate.
    geometrize::Bitmap m_current; ///< The current bitmap.
    float m_score; ///< Score derived from calculating the difference between bitmaps.
};

Model::Model(const geometrize::Bitmap& target, const geometrize::rgba backgroundColor) : d{std::make_unique<Model::ModelImpl>(target, backgroundColor)}
{}

Model::~Model()
{}

void Model::reset(const geometrize::rgba backgroundColor)
{
    d->reset(backgroundColor);
}

std::uint32_t Model::getWidth() const
{
    return d->getWidth();
}

std::uint32_t Model::getHeight() const
{
    return d->getHeight();
}

float Model::getAspectRatio() const
{
    return d->getAspectRatio();
}

std::vector<geometrize::ShapeResult> Model::step(const geometrize::shapes::ShapeTypes shapeTypes, const std::uint8_t alpha, const std::uint32_t repeats)
{
    return d->step(shapeTypes, alpha, repeats);
}

geometrize::ShapeResult Model::drawShape(std::shared_ptr<geometrize::Shape> shape, const std::uint8_t alpha)
{
    return d->drawShape(shape, alpha);
}

geometrize::Bitmap& Model::getCurrent()
{
    return d->getCurrent();
}

}
