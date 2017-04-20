#include "shapeutil.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "../rasterizer/scanline.h"

namespace geometrize
{

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

std::vector<geometrize::Scanline> scanlinesForPolygon(const std::vector<std::pair<std::int32_t, std::int32_t>>& points)
{
    std::vector<geometrize::Scanline> lines;

    std::vector<std::pair<std::int32_t, std::int32_t>> edges;
    for(std::size_t i = 0; i < points.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> p1{points[i]};
        const std::pair<std::int32_t, std::int32_t> p2{(i == (points.size() - 1)) ? points[0U] : points[i + 1U]};
        const std::vector<std::pair<std::int32_t, std::int32_t>> p1p2{geometrize::bresenham(p1.first, p1.second, p2.first, p2.second)};
        edges.insert(edges.end(), p1p2.begin(), p1p2.end());
    }

    std::map<std::int32_t, std::set<std::int32_t>> yToXs;
    for(std::pair<std::int32_t, std::int32_t> point : edges) {
        yToXs[point.second].insert(point.first);
    }

    for(const auto& it : yToXs) {
        const geometrize::Scanline scanline(it.first,
        *(std::min_element(it.second.begin(), it.second.end())),
        *(std::max_element(it.second.begin(), it.second.end())));
        lines.push_back(scanline);
    }

    return lines;
}

}
