#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The SplineInterpolationMode enum specifies the type of interpolation a set of control points shall be rendered with.
 */
enum class SplineInterpolationMode
{
    CARDINAL = 1U,
    MONOTONE = 2U,
    BASIS = 4U,
    LINEAR = 8U,
    STEP_BEFORE = 16U,
    STEP_AFTER = 32U,
    SPLINE_INTERPOLATION_MODE_COUNT = 6U
};

/**
 * @brief The Spline class represents an open spline.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Spline : public Shape
{
public:
    /**
     * @brief Spline Creates a new spline.
     * @param xBound xBound	The x-bound of the whole canvas.
     * @param yBound yBound	The y-bound of the whole canvas.
     */
    Spline(std::int32_t xBound, std::int32_t yBound);

    virtual std::shared_ptr<geometrize::Shape> clone() const;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual geometrize::shapes::ShapeTypes getType() const override;
    virtual std::vector<std::int32_t> getShapeData() const override;

private:
    const std::int32_t m_xBound; ///< The x-bound of the whole canvas.
    const std::int32_t m_yBound; ///< The y-bound of the whole canvas.

    SplineInterpolationMode m_interpolationMode; ///< The interpolation mode for the spline.
    std::vector<std::pair<std::int32_t, std::int32_t>> m_controlPoints; ///< The control points of the spline.
};

}
