#include "commonutil.h"

#include <assert.h>
#include <cstdint>
#include <random>

#include "bitmap/bitmap.h"
#include "bitmap/rgba.h"

namespace geometrize
{

namespace commonutil
{

thread_local static std::mt19937 mt(std::random_device{}());
thread_local static std::uniform_int_distribution<std::int32_t> pick;

void seedRandomGenerator(const std::uint32_t seed)
{
    mt.seed(seed);
}

std::int32_t randomRange(const std::int32_t min, const std::int32_t max)
{
    assert(min <= max);
    return pick(mt, std::uniform_int_distribution<std::int32_t>::param_type{min, max});
}

geometrize::rgba getAverageImageColor(const geometrize::Bitmap& image)
{
    const std::vector<std::uint8_t>& data{image.getDataRef()};
    const std::size_t size{data.size()};
    if(size == 0) {
        return geometrize::rgba{0, 0, 0, 0};
    }

    const std::size_t numPixels{size / 4U};

    std::uint32_t totalRed{0};
    std::uint32_t totalGreen{0};
    std::uint32_t totalBlue{0};
    for(std::size_t i = 0; i < size; i += 4U) {
        totalRed += data[i];
        totalGreen += data[i + 1U];
        totalBlue += data[i + 2U];
    }

    return geometrize::rgba{
        static_cast<std::uint8_t>(totalRed / numPixels),
        static_cast<std::uint8_t>(totalGreen / numPixels),
        static_cast<std::uint8_t>(totalBlue / numPixels),
        static_cast<std::uint8_t>(UINT8_MAX)
    };
}

}

}
