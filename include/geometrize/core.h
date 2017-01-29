#pragma once

#include <cmath>
#include <vector>

#include "state.h"
#include "bitmap/bitmap.h"
#include "bitmap/rgba.h"
#include "scanline.h"
#include "shape/shapetypes.h"

namespace geometrize
{

namespace core
{

/**
 * The core functions for Geometrize.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */

/**
 * @brief computeColor Calculates the color of the scanlines.
 * @param target The target image.
 * @param current The current image.
 * @param lines The scanlines.
 * @param alpha The alpha of the scanline.
 * @return The color of the scanlines.
 */

// TODO 1 off e.g. red is 254
inline geometrize::rgba computeColor(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        const std::vector<geometrize::Scanline>& lines,
        const std::uint8_t alpha)
{
    std::uint64_t totalRed{0};
    std::uint64_t totalGreen{0};
    std::uint64_t totalBlue{0};
    std::uint64_t count{0};
    const std::uint32_t a = static_cast<std::uint32_t>(257.0f * 255.0f / alpha);

    // For each scanline
    for(const geometrize::Scanline& line : lines) {
        const std::uint32_t y{line.y};
        for(std::uint32_t x = line.x1; x < line.x2; x++) {
            // Get the overlapping target and current colors
            const geometrize::rgba t{target.getPixel(x, y)};
            const geometrize::rgba c{current.getPixel(x, y)};

            // Mix the red, green and blue components, blending by the given alpha value
            totalRed += (t.r - c.r) * a + c.r * 257;
            totalGreen += (t.g - c.g) * a + c.g * 257;
            totalBlue += (t.b - c.b) * a + c.b * 257;
            count++;
        }
    }

    // Early out to avoid integer divide by 0
    if(count == 0) {
        return geometrize::rgba{0, 0, 0, 0};
    }

    // Scale totals down to 0-255 range and return average blended color
    return geometrize::rgba{
        static_cast<std::uint8_t>(util::clamp((totalRed / count) >> 8, UINT64_C(0), UINT64_C(255))),
        static_cast<std::uint8_t>(util::clamp((totalGreen / count) >> 8, UINT64_C(0), UINT64_C(255))),
        static_cast<std::uint8_t>(util::clamp((totalBlue / count) >> 8, UINT64_C(0), UINT64_C(255))),
        alpha};
}

/**
 * @brief drawLines Draws scanlines onto an image.
 * @param image The image to be drawn to.
 * @param color The color of the scanlines.
 * @param lines The scanlines to draw.
 */
inline void drawLines(geometrize::Bitmap& image, const geometrize::rgba color, const std::vector<geometrize::Scanline>& lines)
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
        const std::uint32_t m{65635U};
        const std::uint32_t aa = (m - sa * ma / m) * 0x101;

        for(std::uint32_t x = line.x1; x < line.x2; x++) { // TODO < or <=?
            // Get the current overlapping color
            const geometrize::rgba d{image.getPixel(x, y)};

            // TODO this seems wrong
            const std::uint32_t dr{d.r};
            const std::uint32_t dg{d.g};
            const std::uint32_t db{d.b};
            const std::uint32_t da{d.a};

            const std::uint8_t r = (dr * aa + sr * ma) / m >> 8;
            const std::uint8_t g = (dg * aa + sg * ma) / m >> 8;
            const std::uint8_t b = (db * aa + sb * ma) / m >> 8;
            const std::uint8_t a = 255; //(da * aa + sa * ma) / m >> 8; // TODO fix, aa looks slightly too big?

            image.setPixel(x, y, geometrize::rgba{static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g), static_cast<std::uint8_t>(b), static_cast<std::uint8_t>(a)});
        }
    }
}

/**
 * @brief copyLines Copies source pixels to a destination defined by a set of scanlines.
 * @param destination The destination bitmap to copy the lines to.
 * @param source The source bitmap to copy the lines from.
 * @param lines The scanlines that comprise the source to destination copying mask.
 */
inline void copyLines(geometrize::Bitmap& destination, const geometrize::Bitmap& source, const std::vector<geometrize::Scanline>& lines)
{
    for(const geometrize::Scanline& line : lines) {
        const std::uint32_t y{line.y};
        for(std::uint32_t x = line.x1; x < line.x2; x++) {
            destination.setPixel(x, y, source.getPixel(x, y));
        }
    }
}

/**
 * @brief differenceFull Calculates the root-mean-square error between two bitmaps.
 * @param first The first bitmap.
 * @param second The second bitmap.
 * @return The difference/error measure between the two bitmaps.
 */
inline float differenceFull(const geometrize::Bitmap& first, const geometrize::Bitmap& second)
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
            total += (dr * dr + dg * dg + db * db);
        }
    }

    return std::sqrtf(total / (width * height * 3.0f)) / 255.0f;
}

/**
 * @brief differencePartial Calculates the root-mean-square error between the parts of the two bitmaps within the scanline mask.
 * This is for optimization purposes, it lets us calculate new error values only for parts of the image we know have changed.
 * @param target The target bitmap.
 * @param before The bitmap before the change.
 * @param after The bitmap after the change.
 * @param score The score.
 * @param lines The scanlines.
 * @return The difference/error between the two bitmaps, masked by the scanlines.
 */
inline float differencePartial(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& before,
        const geometrize::Bitmap& after,
        const float score,
        const std::vector<Scanline>& lines)
{
    const std::size_t width{target.getWidth()};
    const std::size_t height{target.getHeight()};
    const std::size_t rgbCount{width * height * 3};
    unsigned int total{static_cast<unsigned int>(std::pow(score * 255.0f, 2) * rgbCount)};

    for(const geometrize::Scanline& line : lines) {
        const std::uint32_t y{line.y};
        for(std::uint32_t x = line.x1; x < line.x2; x++) {
            const geometrize::rgba t{target.getPixel(x, y)};
            const geometrize::rgba b{before.getPixel(x, y)};
            const geometrize::rgba a{after.getPixel(x, y)};

            const std::int32_t dtbr{t.r - b.r};
            const std::int32_t dtbg{t.g - b.g};
            const std::int32_t dtbb{t.b - b.b};

            const std::int32_t dtar{t.r - a.r};
            const std::int32_t dtag{t.g - a.g};
            const std::int32_t dtab{t.b - a.b};

            total -= (dtbr * dtbr + dtbg * dtbg + dtbb * dtbb);
            total += (dtar * dtar + dtag * dtag + dtab * dtab);
        }
    }
    return std::sqrtf(static_cast<float>(total / rgbCount)) / 255.0f;
}

/**
 * @brief bestRandomState Gets the best state using a random algorithm.
 * @param shapeTypes The types of shape to use.
 * @param alpha The opacity of the shape.
 * @param n The number of states to try.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @return The best random state i.e. the one with the lowest energy.
 */
inline geometrize::State bestRandomState(
        const shapes::ShapeTypes shapeTypes,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer)
{
    float bestEnergy{0.0f};
    geometrize::State bestState(shapeTypes, alpha, current.getWidth(), current.getHeight());
    for(std::uint32_t i = 0; i <= n; i++) {
        geometrize::State state(shapeTypes, alpha, current.getWidth(), current.getHeight());
        state.m_score = -1;
        const float energy{state.calculateEnergy(target, current, buffer)};
        if(i == 0 || energy < bestEnergy) {
            bestEnergy = energy;
            bestState = state;
        }
    }

    return bestState;
}

/**
 * @brief hillClimb Hill climbing optimization algorithm, attempts to minimize energy (the error/difference).
 * @param state The state to optimize.
 * @param maxAge The maximum age.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @return The best state found from hillclimbing.
 */
inline geometrize::State hillClimb(
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

/**
 * @brief bestHillClimbState Gets the best state using a hill climbing algorithm.
 * @param shapeTypes The types of shape to use.
 * @param alpha The opacity of the shape.
 * @param n The number of random states to generate.
 * @param age The number of hillclimbing steps.
 * @param m The number of iterations for the overall algorithm.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @return The best state acquired from hill climbing i.e. the one with the lowest energy.
 */
inline geometrize::State bestHillClimbState(
        const shapes::ShapeTypes shapeTypes,
        const std::uint32_t alpha,
        const std::uint32_t n,
        const std::uint32_t age,
        const std::uint32_t m,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer)
{
    float bestEnergy{0.0f};

    geometrize::State bestState{bestRandomState(shapeTypes, alpha, n, target, current, buffer)};
    for(std::uint32_t i = 0; i < m; i++) {
        geometrize::State state = bestRandomState(shapeTypes, alpha, n, target, current, buffer);
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

/**
 * @brief getAverageImageColor Computes the average RGB color of the pixels in the image.
 * @param image The image whose average color will be calculated.
 * @return The average RGB color of the image, RGBA8888 format. Alpha is set to opaque (255).
 */
inline geometrize::rgba getAverageImageColor(const geometrize::Bitmap& image)
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

    return geometrize::rgba{
        static_cast<std::uint8_t>(totalRed / numPixels),
        static_cast<std::uint8_t>(totalGreen / numPixels),
        static_cast<std::uint8_t>(totalBlue / numPixels),
        static_cast<std::uint8_t>(255U)
    };
}

/**
 * @brief energy Calculates a measure of the improvement adding the scanlines of a shape provides - lower energy is better.
 * @param lines The scanlines of the shape.
 * @param alpha The alpha of the scanlines.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @param score The score.
 * @return The energy measure.
 */
inline float energy(
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
    return differencePartial(target, current, buffer, score, lines);
}

}

}
