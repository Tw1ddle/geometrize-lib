#include "core.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "bitmap/bitmap.h"
#include "bitmap/rgba.h"
#include "commonutil.h"
#include "rasterizer/rasterizer.h"
#include "rasterizer/scanline.h"
#include "shape/shape.h"
#include "state.h"

namespace
{

/**
* @brief hillClimb Hill climbing optimization algorithm, attempts to minimize energy (the error/difference).
* @param state The state to optimize.
* @param maxAge The maximum age.
* @param target The target bitmap.
* @param current The current bitmap.
* @param buffer The buffer bitmap.
* @param lastScore The last score.
* @return The best state found from hillclimbing.
*/
geometrize::State hillClimb(
        const geometrize::State& state,
        const std::uint32_t maxAge,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const double lastScore,
        const geometrize::core::EnergyFunction& energyFunction)
{
    geometrize::State s(state);
    geometrize::State bestState(state);
    double bestEnergy{bestState.m_score};

    std::uint32_t age{0};
    while(age < maxAge) {
        const geometrize::State undo{s.mutate()};
        s.m_score = energyFunction(s.m_shape->rasterize(*s.m_shape), s.m_alpha, target, current, buffer, lastScore);
        const double energy = s.m_score;
        if(energy >= bestEnergy) {
            s = undo;
        } else {
            bestEnergy = energy;
            bestState = s;
            age = -1;
        }
        age++;
    }

    return bestState;
}

/**
* @brief bestRandomState Gets the best state using a random algorithm.
* @param shapeCreator A function that will create the shapes that will be chosen from.
* @param alpha The opacity of the shape.
* @param n The number of states to try.
* @param target The target bitmap.
* @param current The current bitmap.
* @param buffer The buffer bitmap.
* @param lastScore The last score.
* @return The best random state i.e. the one with the lowest energy.
*/
geometrize::State bestRandomState(
        const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const double lastScore,
        const geometrize::core::EnergyFunction& energyFunction)
{
    geometrize::State bestState(shapeCreator(), alpha);
    bestState.m_score = energyFunction(geometrize::trimScanlines(bestState.m_shape->rasterize(*bestState.m_shape), target.getWidth(), target.getHeight()),
                                       bestState.m_alpha, target, current, buffer, lastScore);
    double bestEnergy = bestState.m_score;

    for(std::uint32_t i = 0; i <= n; i++) {
        geometrize::State state(shapeCreator(), alpha);
        state.m_score = energyFunction(geometrize::trimScanlines(state.m_shape->rasterize(*state.m_shape), target.getWidth(), target.getHeight()),
                                       state.m_alpha, target, current, buffer, lastScore);
        const double energy = state.m_score;
        if(i == 0 || energy < bestEnergy) {
            bestEnergy = energy;
            bestState = state;
        }
    }

    return bestState;
}

}

namespace geometrize
{

namespace core
{

double defaultEnergyFunction(
        const std::vector<geometrize::Scanline>& lines,
        const std::uint32_t alpha,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const double score)
{
    const geometrize::rgba color(geometrize::core::computeColor(target, current, lines, alpha)); // Calculate best color for areas covered by the scanlines
    geometrize::copyLines(buffer, current, lines); // Copy area covered by scanlines to buffer bitmap
    geometrize::drawLines(buffer, color, lines); // Blend scanlines into the buffer using the color calculated earlier
    return geometrize::core::differencePartial(target, current, buffer, score, lines); // Get error measure between areas of current and modified buffers covered by scanlines
}

geometrize::rgba computeColor(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        const std::vector<geometrize::Scanline>& lines,
        const std::uint8_t alpha)
{
    std::int64_t totalRed{0};
    std::int64_t totalGreen{0};
    std::int64_t totalBlue{0};
    std::int64_t count{0};
    const std::int32_t a{static_cast<std::int32_t>(257.0f * 255.0f / static_cast<float>(alpha))};

    // For each scanline
    for(const geometrize::Scanline& line : lines) {
        const std::int32_t y{line.y};
        for(std::int32_t x = line.x1; x <= line.x2; x++) {
            // Get the overlapping target and current colors
            const geometrize::rgba t(target.getPixel(x, y));
            const geometrize::rgba c(current.getPixel(x, y));

            const std::int32_t tr{t.r};
            const std::int32_t tg{t.g};
            const std::int32_t tb{t.b};
            const std::int32_t cr{c.r};
            const std::int32_t cg{c.g};
            const std::int32_t cb{c.b};

            // Mix the red, green and blue components, blending by the given alpha value
            totalRed += static_cast<std::int64_t>((tr - cr) * a + cr * 257);
            totalGreen += static_cast<std::int64_t>((tg - cg) * a + cg * 257);
            totalBlue += static_cast<std::int64_t>((tb - cb) * a + cb * 257);
            count++;
        }
    }

    // Early out to avoid integer divide by 0
    if(count == 0) {
        return geometrize::rgba{0, 0, 0, 0};
    }

    const std::int32_t rr{static_cast<std::int32_t>(totalRed / count) >> 8};
    const std::int32_t gg{static_cast<std::int32_t>(totalGreen / count) >> 8};
    const std::int32_t bb{static_cast<std::int32_t>(totalBlue / count) >> 8};

    // Scale totals down to 0-255 range and return average blended color
    const std::uint8_t r{static_cast<std::uint8_t>(commonutil::clamp(rr, INT32_C(0), INT32_C(255)))};
    const std::uint8_t g{static_cast<std::uint8_t>(commonutil::clamp(gg, INT32_C(0), INT32_C(255)))};
    const std::uint8_t b{static_cast<std::uint8_t>(commonutil::clamp(bb, INT32_C(0), INT32_C(255)))};

    return geometrize::rgba{r, g, b, alpha};
}

double differenceFull(const geometrize::Bitmap& first, const geometrize::Bitmap& second)
{
    assert(first.getWidth() == second.getWidth());
    assert(first.getHeight() == second.getHeight());

    const std::size_t width{first.getWidth()};
    const std::size_t height{first.getHeight()};
    std::uint64_t total{0};

    for(std::uint32_t y = 0; y < height; y++) {
        for(std::uint32_t x = 0; x < width; x++) {
            const geometrize::rgba f(first.getPixel(x, y));
            const geometrize::rgba s(second.getPixel(x, y));

            const std::int32_t dr = {static_cast<std::int32_t>(f.r) - static_cast<std::int32_t>(s.r)};
            const std::int32_t dg = {static_cast<std::int32_t>(f.g) - static_cast<std::int32_t>(s.g)};
            const std::int32_t db = {static_cast<std::int32_t>(f.b) - static_cast<std::int32_t>(s.b)};
            const std::int32_t da = {static_cast<std::int32_t>(f.a) - static_cast<std::int32_t>(s.a)};
            total += (dr * dr + dg * dg + db * db + da * da);
        }
    }
    return std::sqrt(static_cast<double>(total) / (static_cast<double>(width) * static_cast<double>(height) * 4.0)) / 255.0;
}

double differencePartial(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& before,
        const geometrize::Bitmap& after,
        const double score,
        const std::vector<Scanline>& lines)
{
    const std::size_t rgbaCount{target.getWidth() * target.getHeight() * 4U};
    std::uint64_t total{static_cast<std::uint64_t>((score * 255.0) * (score * 255.0) * rgbaCount)};
    for(const geometrize::Scanline& line : lines) {
        const std::int32_t y{line.y};
        for(std::int32_t x = line.x1; x <= line.x2; x++) {
            const geometrize::rgba t(target.getPixel(x, y));
            const geometrize::rgba b(before.getPixel(x, y));
            const geometrize::rgba a(after.getPixel(x, y));

            const std::int32_t dtbr{static_cast<std::int32_t>(t.r) - static_cast<std::int32_t>(b.r)};
            const std::int32_t dtbg{static_cast<std::int32_t>(t.g) - static_cast<std::int32_t>(b.g)};
            const std::int32_t dtbb{static_cast<std::int32_t>(t.b) - static_cast<std::int32_t>(b.b)};
            const std::int32_t dtba{static_cast<std::int32_t>(t.a) - static_cast<std::int32_t>(b.a)};

            const std::int32_t dtar{static_cast<std::int32_t>(t.r) - static_cast<std::int32_t>(a.r)};
            const std::int32_t dtag{static_cast<std::int32_t>(t.g) - static_cast<std::int32_t>(a.g)};
            const std::int32_t dtab{static_cast<std::int32_t>(t.b) - static_cast<std::int32_t>(a.b)};
            const std::int32_t dtaa{static_cast<std::int32_t>(t.a) - static_cast<std::int32_t>(a.a)};

            total -= static_cast<std::uint64_t>(dtbr * dtbr + dtbg * dtbg + dtbb * dtbb + dtba * dtba);
            total += static_cast<std::uint64_t>(dtar * dtar + dtag * dtag + dtab * dtab + dtaa * dtaa);
        }
    }

    const double result{std::sqrt(static_cast<double>(total) / static_cast<double>(rgbaCount)) / 255.0};
    return result;
}

geometrize::State bestHillClimbState(
        const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const std::uint32_t age,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const double lastScore,
        const EnergyFunction& customEnergyFunction)
{
    const EnergyFunction& e = customEnergyFunction ? customEnergyFunction : geometrize::core::defaultEnergyFunction;

    const geometrize::State state{bestRandomState(shapeCreator, alpha, n, target, current, buffer, lastScore, e)};
    return ::hillClimb(state, age, target, current, buffer, lastScore, e);
}

}

}
