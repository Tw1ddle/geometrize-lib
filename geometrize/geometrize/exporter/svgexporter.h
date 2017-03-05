#pragma once

#include <cstdint>
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
 * @brief exportSvg Exports shape data to an SVG image.
 * @param data The shape data to export.
 * @param width The width of the SVG image.
 * @param height The height of the SVG image.
 * @return A string representing the SVG image.
 */
std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height);

}

}
