#include "shapefactory.h"

#include <assert.h>
#include <bitset>
#include <cstdint>
#include <memory>

#include "shape.h"
#include "circle.h"
#include "ellipse.h"
#include "line.h"
#include "spline.h"
#include "rectangle.h"
#include "rotatedellipse.h"
#include "rotatedrectangle.h"
#include "shapetypes.h"
#include "triangle.h"
#include "../commonutil.h"

namespace geometrize
{

std::shared_ptr<geometrize::Shape> create(const geometrize::Model& model, geometrize::shapes::ShapeTypes t, const std::uint32_t xBound, const std::uint32_t yBound)
{
    switch(t) {
        case geometrize::shapes::ShapeTypes::SPLINE:
            return std::make_shared<geometrize::Spline>(model, xBound, yBound);
        case geometrize::shapes::ShapeTypes::CIRCLE:
            return std::make_shared<geometrize::Circle>(model, xBound, yBound);
        case geometrize::shapes::ShapeTypes::ELLIPSE:
            return std::make_shared<geometrize::Ellipse>(model, xBound, yBound);
        case geometrize::shapes::ShapeTypes::ROTATED_ELLIPSE:
            return std::make_shared<geometrize::RotatedEllipse>(model, xBound, yBound);
        case geometrize::shapes::ShapeTypes::ROTATED_RECTANGLE:
            return std::make_shared<geometrize::RotatedRectangle>(model, xBound, yBound);
        case geometrize::shapes::ShapeTypes::TRIANGLE:
            return std::make_shared<geometrize::Triangle>(model, xBound, yBound);
        case geometrize::shapes::ShapeTypes::RECTANGLE:
            return std::make_shared<geometrize::Rectangle>(model, xBound, yBound);
        case geometrize::shapes::ShapeTypes::LINE:
            return std::make_shared<geometrize::Line>(model, xBound, yBound);
        default:
            assert(0 && "Bad shape value");
    };

    assert(0 && "Unhandled shape type encountered");
    return std::make_shared<geometrize::Rectangle>(model, xBound, yBound);
}

std::shared_ptr<geometrize::Shape> randomShape(const geometrize::Model& model, const std::uint32_t xBound, const std::uint32_t yBound)
{
    return create(model, geometrize::shapes::allShapes[commonutil::randomRange(0, static_cast<int>(geometrize::shapes::allShapes.size()) - 1)], xBound, yBound);
}

std::shared_ptr<geometrize::Shape> randomShapeOf(const geometrize::Model& model, const shapes::ShapeTypes types, const std::uint32_t xBound, const std::uint32_t yBound)
{
    std::vector<shapes::ShapeTypes> typeVector;
    for(shapes::ShapeTypes type : shapes::allShapes) {
        if((type & types) == type) {
            typeVector.push_back(type);
        }
    }

    if(typeVector.size() == 0) {
        return randomShape(model, xBound, yBound); // If there are no types specified, create one randomly
    }

    return create(model, typeVector.at(commonutil::randomRange(0, static_cast<std::int32_t>(typeVector.size() - 1))), xBound, yBound);
}

}
