#pragma once

#include <string>
#include <vector>

#include "../shaperesult.h"

namespace geometrize
{

namespace exporter
{

/**
 * @brief exportShapeArray Exports shape data to a compact array-style format.
 * @param data The shape data to export.
 * @return A string containing the exported data.
 */
inline std::string exportShapeArray(const std::vector<ShapeResult>& data)
{
    std::string result;

    for(const ShapeResult& s : data) {
        // TODO write shape attribs, type and vertex positions
    }

    return result;
}

}

}
