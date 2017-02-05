#include "commonutil.h"

#include <assert.h>
#include <cstdint>
#include <random>

namespace geometrize
{

namespace commonutil
{

std::int32_t randomRange(const std::int32_t min, const std::int32_t max)
{
    assert(min <= max);
    return min + (rand() % (max - min + 1));
}

}

}
