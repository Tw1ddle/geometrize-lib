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
    thread_local static std::mt19937 mt(std::random_device{}());
    thread_local static std::uniform_int_distribution<std::int32_t> pick;
    return pick(mt, std::uniform_int_distribution<std::int32_t>::param_type{min, max});
}

std::vector<std::pair<std::int32_t, std::int32_t>> bresenham(std::int32_t x1, std::int32_t y1, const std::int32_t x2, const std::int32_t y2)
{
    std::int32_t dx{x2 - x1};
    const std::int8_t ix{(dx > 0) - (dx < 0)};
    dx = std::abs(dx) << 1;

    std::int32_t dy{y2 - y1};
    const std::int8_t iy{(dy > 0) - (dy < 0)};
    dy = std::abs(dy) << 1;

    std::vector<std::pair<std::int32_t, std::int32_t>> points;
    points.push_back(std::make_pair(x1, y1));

    if (dx >= dy) {
        std::int32_t error(dy - (dx >> 1));
        while (x1 != x2) {
            if (error >= 0 && (error || (ix > 0))) {
                error -= dx;
                y1 += iy;
            }

            error += dy;
            x1 += ix;

            points.push_back(std::make_pair(x1, y1));
        }
    } else {
        std::int32_t error(dx - (dy >> 1));
        while (y1 != y2) {
            if (error >= 0 && (error || (iy > 0))) {
                error -= dy;
                x1 += ix;
            }

            error += dx;
            y1 += iy;

            points.push_back(std::make_pair(x1, y1));
        }
    }

    return points;
}

}

}
