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
#include "state.h"

namespace geometrize
{

namespace core
{

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

float differenceFull(const geometrize::Bitmap& first, const geometrize::Bitmap& second)
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
    return std::sqrt(static_cast<float>(total) / (static_cast<float>(width) * static_cast<float>(height) * 4.0f)) / 255.0f;
}

float differencePartial(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& before,
        const geometrize::Bitmap& after,
        const float score,
        const std::vector<Scanline>& lines)
{
    const std::size_t rgbaCount{target.getWidth() * target.getHeight() * 4U};
    std::uint64_t total{static_cast<std::uint64_t>((score * 255.0f) * (score * 255.0f) * rgbaCount)};
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

    const float result{std::sqrt(static_cast<float>(total) / static_cast<float>(rgbaCount)) / 255.0f};

    // NOTE needs work. This is a workaround because when score/energy is tiny total can unintentionally underflow
    if(result > 1.0f) {
        return score;
    }
    return result;
}

geometrize::State bestRandomState(
        const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const float lastScore)
{
    geometrize::State bestState(shapeCreator(), alpha);
    float bestEnergy{bestState.calculateEnergy(target, current, buffer, lastScore)};

    for(std::uint32_t i = 0; i <= n; i++) {
        geometrize::State state(shapeCreator(), alpha);

        const float energy{state.calculateEnergy(target, current, buffer, lastScore)};
        if(i == 0 || energy < bestEnergy) {
            bestEnergy = energy;
            bestState = state;
        }
    }

    return bestState;
}

geometrize::State hillClimb(
        const geometrize::State& state,
        const std::uint32_t maxAge,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const float lastScore)
{
    geometrize::State s(state);
    geometrize::State bestState(state);
    float bestEnergy{bestState.m_score};

    std::uint32_t age{0};
    while(age < maxAge) {
        const geometrize::State undo{s.mutate()};
        const float energy{s.calculateEnergy(target, current, buffer, lastScore)};
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

geometrize::State bestHillClimbState(
        const std::function<std::shared_ptr<geometrize::Shape>(void)>& shapeCreator,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const std::uint32_t age,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const float lastScore)
{
    const geometrize::State state{bestRandomState(shapeCreator, alpha, n, target, current, buffer, lastScore)};
    return hillClimb(state, age, target, current, buffer, lastScore);
}

float energy(
        const std::vector<geometrize::Scanline>& lines,
        const std::uint32_t alpha,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const float score)
{
    const geometrize::rgba color(computeColor(target, current, lines, alpha)); // Calculate best color for areas covered by the scanlines
    geometrize::copyLines(buffer, current, lines); // Copy area covered by scanlines to buffer bitmap
    geometrize::drawLines(buffer, color, lines); // Blend scanlines into the buffer using the color calculated earlier
    return differencePartial(target, current, buffer, score, lines); // Get error measure between areas of current and modified buffers covered by scanlines
}

}

}
