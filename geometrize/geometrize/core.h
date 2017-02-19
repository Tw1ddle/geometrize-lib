#pragma once

#include <cstdint>
#include <vector>

#include "bitmap/rgba.h"
#include "scanline.h"
#include "shape/shapetypes.h"
#include "state.h"

namespace geometrize
{
class Bitmap;
}

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
geometrize::rgba computeColor(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        const std::vector<geometrize::Scanline>& lines,
        std::uint8_t alpha);

/**
 * @brief drawLines Draws scanlines onto an image.
 * @param image The image to be drawn to.
 * @param color The color of the scanlines.
 * @param lines The scanlines to draw.
 */
void drawLines(geometrize::Bitmap& image, geometrize::rgba color, const std::vector<geometrize::Scanline>& lines);

/**
 * @brief copyLines Copies source pixels to a destination defined by a set of scanlines.
 * @param destination The destination bitmap to copy the lines to.
 * @param source The source bitmap to copy the lines from.
 * @param lines The scanlines that comprise the source to destination copying mask.
 */
void copyLines(geometrize::Bitmap& destination, const geometrize::Bitmap& source, const std::vector<geometrize::Scanline>& lines);

/**
 * @brief differenceFull Calculates the root-mean-square error between two bitmaps.
 * @param first The first bitmap.
 * @param second The second bitmap.
 * @return The difference/error measure between the two bitmaps.
 */
float differenceFull(const geometrize::Bitmap& first, const geometrize::Bitmap& second);

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
float differencePartial(
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& before,
        const geometrize::Bitmap& after,
        float score,
        const std::vector<Scanline>& lines);

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
geometrize::State bestRandomState(
        const geometrize::shapes::ShapeTypes shapeTypes,
        std::uint32_t alpha,
        std::uint32_t n,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer);

/**
 * @brief hillClimb Hill climbing optimization algorithm, attempts to minimize energy (the error/difference).
 * @param state The state to optimize.
 * @param maxAge The maximum age.
 * @param target The target bitmap.
 * @param current The current bitmap.
 * @param buffer The buffer bitmap.
 * @return The best state found from hillclimbing.
 */
geometrize::State hillClimb(
        const geometrize::State& state,
        std::uint32_t maxAge,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer);

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
geometrize::State bestHillClimbState(
        const geometrize::shapes::ShapeTypes shapeTypes,
        std::uint32_t alpha,
        std::uint32_t n,
        std::uint32_t age,
        std::uint32_t m,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer);

/**
 * @brief getAverageImageColor Computes the average RGB color of the pixels in the image.
 * @param image The image whose average color will be calculated.
 * @return The average RGB color of the image, RGBA8888 format. Alpha is set to opaque (255).
 */
geometrize::rgba getAverageImageColor(const geometrize::Bitmap& image);

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
float energy(
        const std::vector<geometrize::Scanline>& lines,
        std::uint32_t alpha,
        const geometrize::Bitmap& target,
        const geometrize::Bitmap& current,
        geometrize::Bitmap& buffer,
        float score);

}

}
