#include "shapeutil.h"

#include <cstdint>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "../commonutil.h"
#include "../scanline.h"

namespace geometrize
{

std::vector<geometrize::Scanline> scanlinesForPolygon(const std::vector<std::pair<std::int32_t, std::int32_t>>& points)
{
    std::vector<geometrize::Scanline> lines;

    std::vector<std::pair<std::int32_t, std::int32_t>> edges;
    for(std::size_t i = 0; i < points.size(); i++) {
        const std::pair<std::int32_t, std::int32_t> p1{points[i]};
        const std::pair<std::int32_t, std::int32_t> p2{(i == (points.size() - 1)) ? points[0U] : points[i + 1U]};
        const std::vector<std::pair<std::int32_t, std::int32_t>> p1p2{commonutil::bresenham(p1.first, p1.second, p2.first, p2.second)};
        edges.insert(edges.end(), p1p2.begin(), p1p2.end());
    }

    std::map<std::int32_t, std::set<std::int32_t>> yToXs;
    for(std::pair<std::int32_t, std::int32_t> point : edges) {
        yToXs[point.second].insert(point.first);
    }

    for(const auto& it : yToXs) {
        const geometrize::Scanline scanline(it.first,
        *(std::min_element(it.second.begin(), it.second.end())),
        *(std::max_element(it.second.begin(), it.second.end())),
        0xFFFF);
        lines.push_back(scanline);
    }

    return lines;
}

}
