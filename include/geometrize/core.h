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
inline rgba computeColor(const Bitmap& target, const Bitmap& current, const std::vector<Scanline>& lines, const unsigned char alpha)
{
    long long int totalRed{0};
    long long int totalGreen{0};
    long long int totalBlue{0};
    long long int count{0};

    const int a = 257.0f * 255.0f / alpha;

    // For each scanline
    for(const Scanline& line : lines) {
        const int y{line.y};
        for(int x = line.x1; x < line.x2; x++) {
            // Get the overlapping target and current colors
            const rgba t{target.getPixel(x, y)};
            const rgba c{current.getPixel(x, y)};

            // Mix the red, green and blue components, blending by the given alpha value
            totalRed += (t.r - c.r) * a + c.r * 257;
            totalGreen += (t.g - c.g) * a + c.g * 257;
            totalBlue += (t.b - c.b) * a + c.b * 257;
            count++;
        }
    }

    // Early out to avoid integer divide by 0
    if(count == 0) {
        return rgba{0, 0, 0, 0};
    }

    // Scale totals down to 0-255 range and return average blended color
    return rgba{
        static_cast<unsigned char>(util::clamp((totalRed / count) >> 8, 0LL, 255LL)),
        static_cast<unsigned char>(util::clamp((totalGreen / count) >> 8, 0LL, 255LL)),
        static_cast<unsigned char>(util::clamp((totalBlue / count) >> 8, 0LL, 255LL)),
        alpha};
}

/**
 * @brief drawLines Draws scanlines onto an image.
 * @param image The image to be drawn to.
 * @param color The color of the scanlines.
 * @param lines The scanlines to draw.
 */
inline void drawLines(Bitmap& image, const rgba color, const std::vector<Scanline>& lines)
{
    // Convert the non-premultiplied color to alpha-premultiplied 16-bits per channel RGBA
    // In other words, scale the rgb color components by the alpha component
    unsigned int sr = color.r;
    sr |= sr << 8;
    sr *= color.a;
    sr /= 0xFF;
    unsigned int sg = color.g;
    sg |= sg << 8;
    sg *= color.a;
    sg /= 0xFF;
    unsigned int sb = color.b;
    sb |= sb << 8;
    sb *= color.a;
    sb /= 0xFF;
    unsigned int sa = color.a;
    sa |= sa << 8;

    // For each scanline
    for(const Scanline& line : lines) {
        const int y{line.y};
        const int ma{line.alpha};
        const int m{65635};
        const int aa = (m - sa * ma / m) * 0x101;

        for(int x = line.x1; x < line.x2; x++) { // TODO < or <=?
            // Get the current overlapping color
            const rgba d{image.getPixel(x, y)};

            // TODO this seems wrong
            const unsigned int dr{d.r};
            const unsigned int dg{d.g};
            const unsigned int db{d.b};
            const unsigned int da{d.a};

            const unsigned char r = (dr * aa + sr * ma) / m >> 8;
            const unsigned char g = (dg * aa + sg * ma) / m >> 8;
            const unsigned char b = (db * aa + sb * ma) / m >> 8;
            const unsigned char a = 255; //(da * aa + sa * ma) / m >> 8; // TODO fix, aa looks slightly too big?

            image.setPixel(x, y, rgba{static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), static_cast<unsigned char>(a)});
        }
    }
}

/**
 * @brief copyLines Copies source pixels to a destination defined by a set of scanlines.
 * @param destination The destination bitmap to copy the lines to.
 * @param source The source bitmap to copy the lines from.
 * @param lines The scanlines that comprise the source to destination copying mask.
 */
inline void copyLines(Bitmap& destination, const Bitmap& source, const std::vector<Scanline>& lines)
{
    for(const Scanline& line : lines) {
        const int y{line.y};
        for(int x = line.x1; x < line.x2; x++) {
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
inline float differenceFull(const Bitmap& first, const Bitmap& second)
{
    assert(first.getWidth() == second.getWidth());
    assert(first.getHeight() == second.getHeight());

    const std::size_t width{first.getWidth()};
    const std::size_t height{first.getHeight()};
    unsigned int total{0};

    for(unsigned int y = 0; y < height; y++) {
        for(unsigned int x = 0; x < width; x++) {
            const rgba f{first.getPixel(x, y)};
            const rgba s{second.getPixel(x, y)};

            const int dr = {f.r - s.r};
            const int dg = {f.g - s.g};
            const int db = {f.b - s.b};
            total += (dr * dr + dg * dg + db * db);
        }
    }

    return std::sqrt(total / (width * height * 3.0f)) / 255.0f;
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
inline float differencePartial(const Bitmap& target, const Bitmap& before, const Bitmap& after, const float score, const std::vector<Scanline>& lines)
{
    const std::size_t width{target.getWidth()};
    const std::size_t height{target.getHeight()};
    const std::size_t rgbCount{width * height * 3};
    unsigned int total{static_cast<unsigned int>(std::pow(score * 255.0f, 2) * rgbCount)};

    for(const Scanline& line : lines) {
        const int y{line.y};
        for(int x = line.x1; x < line.x2; x++) {
            const rgba t{target.getPixel(x, y)};
            const rgba b{before.getPixel(x, y)};
            const rgba a{after.getPixel(x, y)};

            const int dtbr{t.r - b.r};
            const int dtbg{t.g - b.g};
            const int dtbb{t.b - b.b};

            const int dtar{t.r - a.r};
            const int dtag{t.g - a.g};
            const int dtab{t.b - a.b};

            total -= (dtbr * dtbr + dtbg * dtbg + dtbb * dtbb);
            total += (dtar * dtar + dtag * dtag + dtab * dtab);
        }
    }
    return std::sqrt(total / rgbCount) / 255;
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
inline State bestRandomState(const shapes::ShapeTypes shapeTypes, const int alpha, const int n, const Bitmap& target, const Bitmap& current, Bitmap& buffer)
{
    float bestEnergy{0.0f};
    State bestState(shapeTypes, alpha, current.getWidth(), current.getHeight());
    for(int i = 0; i <= n; i++) {
        State state(shapeTypes, alpha, current.getWidth(), current.getHeight());
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
inline State hillClimb(const State& state, const unsigned int maxAge, const Bitmap& target, const Bitmap& current, Bitmap& buffer)
{
    State s(state);
    s.m_score = -1;
    State bestState(state);
    bestState.m_score = -1;
    float bestEnergy{s.calculateEnergy(target, current, buffer)};

    unsigned int age{0};
    while(age < maxAge) {
        const State undo{s.mutate()};
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
inline State bestHillClimbState(const shapes::ShapeTypes shapeTypes, const int alpha, const int n, const int age, const int m, const Bitmap& target, const Bitmap& current, Bitmap& buffer)
{
    float bestEnergy{0.0f};

    State bestState{bestRandomState(shapeTypes, alpha, n, target, current, buffer)};
    for(int i = 0; i < m; i++) {
        State state = bestRandomState(shapeTypes, alpha, n, target, current, buffer);
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
inline rgba getAverageImageColor(const Bitmap& image)
{
    const std::vector<unsigned char>& data{image.getDataRef()};
    const std::size_t size{data.size()};
    const std::size_t numPixels{data.size() / 4};

    int totalRed{0};
    int totalGreen{0};
    int totalBlue{0};
    for(std::size_t i = 0; i < size; i += 4) {
        totalRed += data[i];
        totalGreen += data[i + 1];
        totalBlue += data[i + 2];
    }

    return rgba{static_cast<unsigned char>(totalRed / numPixels), static_cast<unsigned char>(totalGreen / numPixels), static_cast<unsigned char>(totalBlue / numPixels), 255};
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
inline float energy(const std::vector<Scanline>& lines, const int alpha, const Bitmap& target, const Bitmap& current, Bitmap& buffer, const float score)
{
    // Calculates the best color for the area covered by the scanlines
    const rgba color{computeColor(target, current, lines, alpha)};
    // Copies the area covered by the scanlines to the buffer bitmap
    copyLines(buffer, current, lines);
    // Blends the scanlines into the buffer bitmap using the best color calculated earlier
    drawLines(buffer, color, lines);
    // Gets the error measure between the parts of the current and the modified buffer texture covered by the scanlines
    return differencePartial(target, current, buffer, score, lines);
}

}

}
