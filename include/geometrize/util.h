#pragma once

#include <algorithm>
#include <assert.h>

namespace geometrize
{

namespace util
{

/**
 * @brief The Random class is a little utility for generating random numbers.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Random {
public:
    /**
     * @brief fastRand A fast implementation of the rand() function from the C library.
     * @return A random integer.
     */
    inline static int fastRand(int seed = 12345) // TODO make it possible to manipulate the seed?
    {
        seed = (214013 * seed + 2531011);
        return (seed >> 16) & 0x7FFF;
    }

    /**
     * @brief randomRange Returns a random integer in the range, inclusive.
     * @param min The lower bound.
     * @param max The upper bound.
     * @return The random integer in the range.
     */
    inline static int randomRange(const int min, const int max)
    {
        assert(min <= max);
        return fastRand() % (max + 1 - min) + min; // Note this is biased
    }
};

/**
 * Clamps a value within a range.
 * @param value The value to clamp.
 * @param lower The lower bound of the range.
 * @param upper The upper bound of the range.
 * @return The clamped value.
 */
template<typename T> T clamp(const T& value, const T& lower, const T& upper)
{
    return (std::max)(lower, (std::min)(value, upper));
}

}

}
