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
 * @brief SVG_STYLE_HOOK A hook that an SVG exporter should use to augment shape styling produced by the getSvgShapeData method.
 */
static const std::string SVG_STYLE_HOOK = "::svg_style_hook::";

/**
 * @brief The SVGExportOptions struct represents the options that can be set for the SVG export.
 */
struct SVGExportOptions
{
    // NOTE currently unused
};

/**
 * @brief getSvgShapeData Gets the SVG data for a single shape. This is just the <rect>/<path> etc block for the shape itself, not a complete SVG image.
 * @param color The color of the shape.
 * @param shape The shape to convert to SVG data.
 * @param options additional options used by the exporter.
 * @return The SVG shape data for the given shape.
 */
std::string getSingleShapeSVGData(const geometrize::rgba& color, const geometrize::Shape& shape, SVGExportOptions options = SVGExportOptions{});

/**
 * @brief exportSVG Exports a single shape as a complete SVG image.
 * @param color The color of the shape to export.
 * @param shape The shape to export.
 * @param width The width of the SVG image.
 * @param height The height of the SVG image.
 * @param options additional options used by the exporter.
 * @return A string representing the SVG image.
 */
std::string exportSingleShapeSVG(const geometrize::rgba& color, const geometrize::Shape& shape, const std::uint32_t width, const std::uint32_t height, SVGExportOptions options = SVGExportOptions{});

/**
 * @brief exportSVG Exports shape data as a complete SVG image.
 * @param data The shape data to export.
 * @param width The width of the SVG image.
 * @param height The height of the SVG image.
 * @param options additional options used by the exporter.
 * @return A string representing the SVG image.
 */
std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height, SVGExportOptions options = SVGExportOptions{});

}

}
