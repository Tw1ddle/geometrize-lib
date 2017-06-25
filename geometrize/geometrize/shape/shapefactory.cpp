#include "shapefactory.h"

#include <bitset>
#include <cassert>
#include <cstdint>
#include <memory>

#include "shape.h"
#include "circle.h"
#include "ellipse.h"
#include "line.h"
#include "polyline.h"
#include "quadraticbezier.h"
#include "rectangle.h"
#include "rotatedellipse.h"
#include "rotatedrectangle.h"
#include "shapetypes.h"
#include "triangle.h"
#include "../commonutil.h"

namespace geometrize
{

std::shared_ptr<geometrize::Shape> create(const geometrize::Model& model, const geometrize::ShapeTypes t)
{
    switch(t) {
        case geometrize::ShapeTypes::POLYLINE:
            return std::make_shared<geometrize::Polyline>(model);
        case geometrize::ShapeTypes::QUADRATIC_BEZIER:
            return std::make_shared<geometrize::QuadraticBezier>(model);
        case geometrize::ShapeTypes::CIRCLE:
            return std::make_shared<geometrize::Circle>(model);
        case geometrize::ShapeTypes::ELLIPSE:
            return std::make_shared<geometrize::Ellipse>(model);
        case geometrize::ShapeTypes::ROTATED_ELLIPSE:
            return std::make_shared<geometrize::RotatedEllipse>(model);
        case geometrize::ShapeTypes::ROTATED_RECTANGLE:
            return std::make_shared<geometrize::RotatedRectangle>(model);
        case geometrize::ShapeTypes::TRIANGLE:
            return std::make_shared<geometrize::Triangle>(model);
        case geometrize::ShapeTypes::RECTANGLE:
            return std::make_shared<geometrize::Rectangle>(model);
        case geometrize::ShapeTypes::LINE:
            return std::make_shared<geometrize::Line>(model);
        default:
            assert(0 && "Bad shape type specified");
    };

    assert(0 && "Unhandled shape type encountered");
    return std::make_shared<geometrize::Rectangle>(model);
}

std::shared_ptr<geometrize::Shape> randomShape(const geometrize::Model& model)
{
    return create(model, geometrize::allShapes[commonutil::randomRange(0, static_cast<int>(geometrize::allShapes.size()) - 1)]);
}

std::shared_ptr<geometrize::Shape> randomShapeOf(const geometrize::Model& model, const ShapeTypes types)
{
    std::vector<ShapeTypes> typeVector;
    for(const ShapeTypes type : geometrize::allShapes) {
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
