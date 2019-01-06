#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../bitmap/rgba.h"

namespace geometrize
{
class Shape;
struct ShapeResult;
}

namespace geometrize
{

namespace exporter
{

/**
 * @brief The SVGExportOptions struct represents the options that can be set for the SVG export.
 */
struct SVGExportOptions
{
    // NOTE currently unused
};

/**
 * @brief exportSVG Exports a single shape to an SVG image.
 * @param color The color of the shape to export.
 * @param shape The shape to export.
 * @param width The width of the SVG image.
 * @param height The height of the SVG image.
 * @param options additional options used by the exporter.
 * @return A string representing the SVG image.
 */
std::string exportSingleShapeSVG(const geometrize::rgba& color, const geometrize::Shape& shape, const std::uint32_t width, const std::uint32_t height, SVGExportOptions options = SVGExportOptions{});

/**
 * @brief exportSVG Exports shape data to an SVG image.
 * @param data The shape data to export.
 * @param width The width of the SVG image.
 * @param height The height of the SVG image.
 * @param options additional options used by the exporter.
 * @return A string representing the SVG image.
 */
std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height, SVGExportOptions options = SVGExportOptions{});

}

}
