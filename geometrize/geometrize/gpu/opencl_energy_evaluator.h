#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../bitmap/rgba.h"
#include "../rasterizer/scanline.h"

namespace geometrize
{
class Bitmap;
}

namespace geometrize
{
namespace gpu
{

/**
 * Evaluates batches of the built-in Geometrize energy function on OpenCL.
 * The implementation loads OpenCL dynamically and becomes an inert CPU
 * fallback when no suitable device or runtime is available.
 */
class OpenClEnergyEvaluator
{
public:
    OpenClEnergyEvaluator(const geometrize::Bitmap& target, const geometrize::Bitmap& current);
    ~OpenClEnergyEvaluator();
    OpenClEnergyEvaluator(const OpenClEnergyEvaluator&) = delete;
    OpenClEnergyEvaluator& operator=(const OpenClEnergyEvaluator&) = delete;

    bool isAvailable() const;
    const std::string& getDeviceName() const;
    const std::string& getLastError() const;

    bool evaluate(
            const std::vector<std::vector<geometrize::Scanline>>& candidateLines,
            std::uint8_t alpha,
            double score,
            std::vector<double>& scores);

    bool commit(const std::vector<geometrize::Scanline>& lines, geometrize::rgba color);
    bool resetCurrent(const geometrize::Bitmap& current);

private:
    class Impl;
    std::unique_ptr<Impl> d;
};

}
}

