#pragma once

#include <string>
#include <vector>

namespace geometrize
{
struct ShapeResult;
}

namespace geometrize
{

namespace exporter
{

/**
 * @brief exportShapeJson Exports shape data to JSON.
 * @param data The shape data to export.
 * @return A string containing the exported JSON.
 */
std::string exportShapeJson(const std::vector<geometrize::ShapeResult>& data);

}

}
