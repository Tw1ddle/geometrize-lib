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

std::shared_ptr<geometrize::Shape> create(const geometrize::ShapeTypes t)
{
    switch(t) {
        case geometrize::ShapeTypes::POLYLINE:
            return std::make_shared<geometrize::Polyline>();
        case geometrize::ShapeTypes::QUADRATIC_BEZIER:
            return std::make_shared<geometrize::QuadraticBezier>();
        case geometrize::ShapeTypes::CIRCLE:
            return std::make_shared<geometrize::Circle>();
        case geometrize::ShapeTypes::ELLIPSE:
            return std::make_shared<geometrize::Ellipse>();
        case geometrize::ShapeTypes::ROTATED_ELLIPSE:
            return std::make_shared<geometrize::RotatedEllipse>();
        case geometrize::ShapeTypes::ROTATED_RECTANGLE:
            return std::make_shared<geometrize::RotatedRectangle>();
        case geometrize::ShapeTypes::TRIANGLE:
            return std::make_shared<geometrize::Triangle>();
        case geometrize::ShapeTypes::RECTANGLE:
            return std::make_shared<geometrize::Rectangle>();
        case geometrize::ShapeTypes::LINE:
            return std::make_shared<geometrize::Line>();
        default:
            assert(0 && "Bad shape type specified");
    };

    assert(0 && "Unhandled shape type encountered");
    return std::make_shared<geometrize::Rectangle>();
}

std::shared_ptr<geometrize::Shape> randomShape()
{
    return create(geometrize::allShapes[commonutil::randomRange(0, static_cast<int>(geometrize::allShapes.size()) - 1)]);
}

std::shared_ptr<geometrize::Shape> randomShapeOf(const ShapeTypes types)
{
    std::vector<ShapeTypes> typeVector;
    for(const ShapeTypes type : geometrize::allShapes) {
        if((type & types) == type) {
            typeVector.push_back(type);
        }
    }

    if(typeVector.size() == 0) {
        return randomShape(); // If there are no types specified, create one randomly
    }

    return create(typeVector.at(commonutil::randomRange(0, static_cast<std::int32_t>(typeVector.size() - 1))));
}

}
