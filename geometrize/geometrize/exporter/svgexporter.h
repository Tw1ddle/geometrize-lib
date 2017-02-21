#pragma once

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
 * @brief exportSvg Exports shape data to an SVG image.
 * @param data The shape data to export.
 * @return A string representing the SVG image.
 */
std::string exportSvg(const std::vector<geometrize::ShapeResult>& data);

}

}
