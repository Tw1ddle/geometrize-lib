#pragma once

#include <cstdint>
#include <memory>

#include "shape.h"

namespace geometrize
{
class Model;
}

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
    /**
     * @brief Polyline Creates a new polyline.
     * @param model The model that created this shape.
     */
    Polyline(const geometrize::Model& model);

    virtual std::shared_ptr<geometrize::Shape> clone() const override;
    virtual std::vector<geometrize::Scanline> rasterize() const override;
    virtual void mutate() override;
    virtual void translate(float x, float y) override;
    virtual void scale(float scaleFactor) override;
    virtual geometrize::ShapeTypes getType() const override;
    virtual std::vector<float> getRawShapeData() const override;
    virtual std::string getSvgShapeData() const override;

    std::vector<std::pair<float, float>> m_points; ///< The points on the polyline.
};

}
