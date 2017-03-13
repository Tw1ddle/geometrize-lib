#include "shapefactory.h"

#include <assert.h>
#include <bitset>
#include <cstdint>
#include <memory>

#include "shape.h"
#include "circle.h"
#include "ellipse.h"
#include "line.h"
#include "polyline.h"
#include "spline.h"
#include "rectangle.h"
#include "rotatedellipse.h"
#include "rotatedrectangle.h"
#include "shapetypes.h"
#include "triangle.h"
#include "../commonutil.h"

namespace geometrize
{

std::shared_ptr<geometrize::Shape> create(const geometrize::Model& model, geometrize::shapes::ShapeTypes t)
{
    switch(t) {
        case geometrize::shapes::ShapeTypes::POLYLINE:
            return std::make_shared<geometrize::Polyline>(model);
        case geometrize::shapes::ShapeTypes::SPLINE:
            return std::make_shared<geometrize::Spline>(model);
        case geometrize::shapes::ShapeTypes::CIRCLE:
            return std::make_shared<geometrize::Circle>(model);
        case geometrize::shapes::ShapeTypes::ELLIPSE:
            return std::make_shared<geometrize::Ellipse>(model);
        case geometrize::shapes::ShapeTypes::ROTATED_ELLIPSE:
            return std::make_shared<geometrize::RotatedEllipse>(model);
        case geometrize::shapes::ShapeTypes::ROTATED_RECTANGLE:
            return std::make_shared<geometrize::RotatedRectangle>(model);
        case geometrize::shapes::ShapeTypes::TRIANGLE:
            return std::make_shared<geometrize::Triangle>(model);
        case geometrize::shapes::ShapeTypes::RECTANGLE:
            return std::make_shared<geometrize::Rectangle>(model);
        case geometrize::shapes::ShapeTypes::LINE:
            return std::make_shared<geometrize::Line>(model);
        default:
            assert(0 && "Bad shape type specified");
    };

    assert(0 && "Unhandled shape type encountered");
    return std::make_shared<geometrize::Rectangle>(model);
}

std::shared_ptr<geometrize::Shape> randomShape(const geometrize::Model& model)
{
    return create(model, geometrize::shapes::allShapes[commonutil::randomRange(0, static_cast<int>(geometrize::shapes::allShapes.size()) - 1)]);
}

std::shared_ptr<geometrize::Shape> randomShapeOf(const geometrize::Model& model, const shapes::ShapeTypes types)
{
    std::vector<shapes::ShapeTypes> typeVector;
    for(shapes::ShapeTypes type : shapes::allShapes) {
        if((type & types) == type) {
            typeVector.push_back(type);
        }
    }

    if(typeVector.size() == 0) {
        return randomShape(model); // If there are no types specified, create one randomly
    }

    return create(model, typeVector.at(commonutil::randomRange(0, static_cast<std::int32_t>(typeVector.size() - 1))));
}

}
