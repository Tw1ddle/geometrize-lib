#pragma once

#include <array>
#include <cstdint>

namespace geometrize
{

namespace shapes
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
    SPLINE = 128U,
    SHAPE_COUNT = 8U
};

///< An array of all shape types.
static const std::array<ShapeTypes, static_cast<std::size_t>(ShapeTypes::SHAPE_COUNT)> allShapes =
{
    ShapeTypes::RECTANGLE,
    ShapeTypes::ROTATED_RECTANGLE,
    ShapeTypes::TRIANGLE,
    ShapeTypes::ELLIPSE,
    ShapeTypes::ROTATED_ELLIPSE,
    ShapeTypes::CIRCLE,
    ShapeTypes::LINE,
    ShapeTypes::SPLINE
};

}

}
