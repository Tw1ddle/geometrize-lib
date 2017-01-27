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
     * @brief randomRange Returns a random integer in the range, inclusive.
     * @param min The lower bound.
     * @param max The upper bound.
     * @return The random integer in the range.
     */
    inline static int randomRange(const int min, const int max)
    {
        assert(min <= max);
        return min + (rand() % (max - min + 1)); // Note this is biased
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
