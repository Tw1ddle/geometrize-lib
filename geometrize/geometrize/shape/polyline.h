#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "shape.h"

namespace geometrize
{

/**
 * @brief The Polyline class represents a polyline.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Polyline : public Shape
{
public:
    Polyline() = default;
    Polyline(const std::vector<std::pair<float, float>>& points);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual geometrize::ShapeTypes getType() const override;

    std::vector<std::pair<float, float>> m_points; ///< The points on the polyline.
};

}
