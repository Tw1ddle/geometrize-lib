#pragma once

#include <array>

namespace geometrize
{

namespace shapes
{

/**
 * @brief The ShapeTypes enum specifies the types of shapes that can be used. These can be combined to produce images composed of multiple primitive types.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
enum ShapeTypes
{
    RECTANGLE = 1U,
    ROTATED_RECTANGLE = 2U,
    TRIANGLE = 4U,
    ELLIPSE = 8U,
    ROTATED_ELLIPSE = 16U,
    CIRCLE = 32U,
    SPLINE = 64U,
    SHAPE_COUNT = 7U
};

static const std::array<ShapeTypes, ShapeTypes::SHAPE_COUNT> allShapes = { RECTANGLE, ROTATED_RECTANGLE, TRIANGLE, ELLIPSE, ROTATED_ELLIPSE, CIRCLE, SPLINE }; ///< An array of all shape types.

}

}
