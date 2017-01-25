#pragma once

#include <string>
#include <vector>

#include "../shaperesult.h"

namespace geometrize
{

namespace exporter
{

/**
 * @brief exportShapeJson Exports shape data to JSON.
 * @param data The shape data to export.
 * @return A string containing the exported JSON.
 */
inline std::string exportShapeJson(const std::vector<ShapeResult>& data)
{
    std::string result;

    for(const ShapeResult& s : data) {
        // TODO write shape attribs, type and vertex positions
    }

    return result;
}

}

}
