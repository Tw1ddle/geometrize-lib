#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace geometrize
{

/**
 * @brief The ShapeTypes enum specifies the types of shapes that can be used. These can be combined to produce images composed of multiple primitive types.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
enum ShapeTypes : std::uint32_t
{
    RECTANGLE = 1U,
    ROTATED_RECTANGLE = 2U,
    TRIANGLE = 4U,
    ELLIPSE = 8U,
    ROTATED_ELLIPSE = 16U,
    CIRCLE = 32U,
    LINE = 64U,
    QUADRATIC_BEZIER = 128U,
    POLYLINE = 256U,
    SHAPE_COUNT = 9U
};

/**
 * @brief allShapes is a convenient array of all of the members of ShapeTypes.
 */
extern const std::array<ShapeTypes, static_cast<std::size_t>(ShapeTypes::SHAPE_COUNT)> allShapes;

/**
 * @brief shapeTypeNames provides a convenient mapping to names of types of shape (all lower case, underscores instead of spaces e.g. rotated_ellipse).
 */
extern const std::vector<std::pair<ShapeTypes, std::string>> shapeTypeNames;

}
