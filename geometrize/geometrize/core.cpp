#include "core.h"

#include <assert.h>
#include <cmath>
#include <cstdint>
#include <vector>

#include "simdpp/simd.h"

#include "bitmap/bitmap.h"
#include "bitmap/rgba.h"
#include "scanline.h"
#include "shape/shapetypes.h"
#include "state.h"
#include "commonutil.h"

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
    const std::int32_t a{static_cast<std::int32_t>(257.0f * 255.0f / alpha)};

    // For each scanline
    for(const geometrize::Scanline& line : lines) {
        const std::uint32_t y{line.y};
        for(std::uint32_t x = line.x1; x <= line.x2; x++) {
            // Get the overlapping target and current colors
            const geometrize::rgba t{target.getPixel(x, y)};
            const geometrize::rgba c{current.getPixel(x, y)};

            const int tr = t.r;
            const int tg = t.g;
            const int tb = t.b;
            const int cr = c.r;
            const int cg = c.g;
            const int cb = c.b;

            // Mix the red, green and blue components, blending by the given alpha value
            totalRed += (std::int64_t)((tr - cr) * a + cr * 257);
            totalGreen += (std::int64_t)((tg - cg) * a + cg * 257);
            totalBlue += (std::int64_t)((tb - cb) * a + cb * 257);
            count++;
        }
    }

    // Early out to avoid integer divide by 0
    if(count == 0) {
        return geometrize::rgba{0, 0, 0, 0};
    }

    const std::int32_t rr = INT32_C((totalRed / count) >> 8);
    const std::int32_t gg = INT32_C((totalGreen / count) >> 8);
    const std::int32_t bb = INT32_C((totalBlue / count) >> 8);

    // Scale totals down to 0-255 range and return average blended color
    const std::int32_t r = commonutil::clamp(rr, INT32_C(0), INT32_C(255));
    const std::int32_t g = commonutil::clamp(gg, INT32_C(0), INT32_C(255));
    const std::int32_t b = commonutil::clamp(bb, INT32_C(0), INT32_C(255));

    return geometrize::rgba{static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b), alpha};
}

void drawLines(geometrize::Bitmap& image, const geometrize::rgba color, const std::vector<geometrize::Scanline>& lines)
{
    // Convert the non-premultiplied color to alpha-premultiplied 16-bits per channel RGBA
    // In other words, scale the rgb color components by the alpha component
    std::uint32_t sr = color.r;
    sr |= sr << 8;
    sr *= color.a;
    sr /= 0xFF;
    std::uint32_t sg = color.g;
    sg |= sg << 8;
    sg *= color.a;
    sg /= 0xFF;
    std::uint32_t sb = color.b;
    sb |= sb << 8;
    sb *= color.a;
    sb /= 0xFF;
    std::uint32_t sa = color.a;
    sa |= sa << 8;

    // For each scanline
    for(const geometrize::Scanline& line : lines) {
        const std::uint32_t y{line.y};
        const std::uint32_t ma{line.alpha};
        const std::uint32_t m{UINT16_MAX};
        const std::uint32_t aa = (m - sa * ma / m) * 0x101;

        for(std::uint32_t x = line.x1; x <= line.x2; x++) {
            // Get the current overlapping color
            const geometrize::rgba d{image.getPixel(x, y)};

            const std::uint8_t r = ((d.r * aa + sr * ma) / m) >> 8;
            const std::uint8_t g = ((d.g * aa + sg * ma) / m) >> 8;
            const std::uint8_t b = ((d.b * aa + sb * ma) / m) >> 8;
            const std::uint8_t a = ((d.a * aa + sa * ma) / m) >> 8; // TODO fix, aa looks slightly too big?

            image.setPixel(x, y, geometrize::rgba{static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b), static_cast<std::uint8_t>(a)});
        }
    }
}

void copyLines(geometrize::Bitmap& destination, const geometrize::Bitmap& source, const std::vector<geometrize::Scanline>& lines)
{
    for(const geometrize::Scanline& line : lines) {
        const std::uint32_t y{line.y};
        for(std::uint32_t x = line.x1; x < line.x2; x++) {
            destination.setPixel(x, y, source.getPixel(x, y));
        }
    }
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
            const geometrize::rgba f{first.getPixel(x, y)};
            const geometrize::rgba s{second.getPixel(x, y)};

            const std::int32_t dr = {f.r - s.r};
            const std::int32_t dg = {f.g - s.g};
            const std::int32_t db = {f.b - s.b};
            const std::int32_t da = {f.a - s.a};
            total += (dr * dr + dg * dg + db * db + da * da);
        }
    }
    return std::sqrt(total / (width * height * 4.0f)) / 255.0f;
}

float differencePartial(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& before,
        const geometrize::Bitmap& after,
        const float score,
        const std::vector<Scanline>& lines)
{
    const std::size_t width{target.getWidth()};
    const std::size_t height{target.getHeight()};
    const std::size_t rgbaCount{width * height * 4};
    std::uint64_t total{static_cast<std::uint32_t>(std::pow(score * 255.0f, 2) * rgbaCount)};

    for(const geometrize::Scanline& line : lines) {
        const std::uint32_t y{line.y};
        for(std::uint32_t x = line.x1; x <= line.x2; x++) {
            const geometrize::rgba t{target.getPixel(x, y)};
            const geometrize::rgba b{before.getPixel(x, y)};
            const geometrize::rgba a{after.getPixel(x, y)};

            // TODO rewrite with SSE and/or multithreading
            const std::int32_t dtbr{t.r - b.r};
            const std::int32_t dtbg{t.g - b.g};
            const std::int32_t dtbb{t.b - b.b};
            const std::int32_t dtba{t.a - b.a};

            const std::int32_t dtar{t.r - a.r};
            const std::int32_t dtag{t.g - a.g};
            const std::int32_t dtab{t.b - a.b};
            const std::int32_t dtaa{t.a - a.a};

            total -= (dtbr * dtbr + dtbg * dtbg + dtbb * dtbb + dtba * dtba);
            total += (dtar * dtar + dtag * dtag + dtab * dtab + dtaa * dtaa);
        }
    }
    return std::sqrt(static_cast<float>(total / rgbaCount)) / 255.0f;
}

geometrize::State bestRandomState(
        const geometrize::Model& model,
        const geometrize::shapes::ShapeTypes shapeTypes,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer)
{
    float bestEnergy{0.0f};
    geometrize::State bestState(model, shapeTypes, alpha, current.getWidth(), current.getHeight());
    for(std::uint32_t i = 0; i <= n; i++) {
        geometrize::State state(model, shapeTypes, alpha, current.getWidth(), current.getHeight());
        state.m_score = -1;
        const float energy{state.calculateEnergy(target, current, buffer)};
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
        geometrize::Bitmap& buffer)
{
    geometrize::State s(state);
    s.m_score = -1;
    geometrize::State bestState(state);
    bestState.m_score = -1;
    float bestEnergy{s.calculateEnergy(target, current, buffer)};

    unsigned int age{0};
    while(age < maxAge) {
        const geometrize::State undo{s.mutate()};
        const float energy{s.calculateEnergy(target, current, buffer)};
        if(energy >= bestEnergy) {
            s = undo;
        } else {
            bestEnergy = energy;
            bestState = s;
            bestState.m_score = -1;
            age = -1;
        }
        age++;
    }

    return bestState;
}

geometrize::State bestHillClimbState(
        const geometrize::Model& model,
        const geometrize::shapes::ShapeTypes shapeTypes,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const std::uint32_t age,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer)
{
    float bestEnergy{0.0f};

    // TODO
    geometrize::State bestState{bestRandomState(model, shapeTypes, alpha, n, target, current, buffer)};
    for(std::uint32_t i = 0; i < 1; i++) {
        geometrize::State state = bestRandomState(model, shapeTypes, alpha, n, target, current, buffer);
        const float before{state.calculateEnergy(target, current, buffer)};
        state = hillClimb(state, age, target, current, buffer);
        const float energy{state.calculateEnergy(target, current, buffer)};
        if(i == 0 || energy < bestEnergy) {
            bestEnergy = energy;
            bestState = state;
        }
    }
    return bestState;
}

geometrize::rgba getAverageImageColor(const geometrize::Bitmap& image)
{
    const std::vector<std::uint8_t>& data{image.getDataRef()};
    const std::size_t size{data.size()};
    const std::size_t numPixels{data.size() / 4};

    std::uint32_t totalRed{0};
    std::uint32_t totalGreen{0};
    std::uint32_t totalBlue{0};
    for(std::size_t i = 0; i < size; i += 4) {
        totalRed += data[i];
        totalGreen += data[i + 1];
        totalBlue += data[i + 2];
    }

    const geometrize::rgba result{
        static_cast<std::uint8_t>(totalRed / numPixels),
        static_cast<std::uint8_t>(totalGreen / numPixels),
        static_cast<std::uint8_t>(totalBlue / numPixels),
        static_cast<std::uint8_t>(UINT8_MAX)
    };

    return result;
}

float energy(
        const std::vector<geometrize::Scanline>& lines,
        const std::uint32_t alpha,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        const float score)
{
    // Calculates the best color for the area covered by the scanlines
    const geometrize::rgba color{computeColor(target, current, lines, alpha)};
    // Copies the area covered by the scanlines to the buffer bitmap
    copyLines(buffer, current, lines);
    // Blends the scanlines into the buffer bitmap using the best color calculated earlier
    drawLines(buffer, color, lines);
    // Gets the error measure between the parts of the current and the modified buffer texture covered by the scanlines
    const float energy{differencePartial(target, current, buffer, score, lines)};

    return energy;
}

}

}
