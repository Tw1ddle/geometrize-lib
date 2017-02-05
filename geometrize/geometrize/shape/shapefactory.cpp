#include "shapefactory.h"

#include <assert.h>
#include <bitset>
#include <cstdint>
#include <memory>

#include "shape.h"
#include "circle.h"
#include "ellipse.h"
#include "rectangle.h"
#include "rotatedellipse.h"
#include "rotatedrectangle.h"
#include "shapetypes.h"
#include "triangle.h"
#include "../commonutil.h"

namespace geometrize
{

geometrize::Shape* create(geometrize::shapes::ShapeTypes t, const std::uint32_t xBound, const std::uint32_t yBound)
{
    switch(t) {
        case geometrize::shapes::ShapeTypes::CIRCLE:
            return new Circle(xBound, yBound);
        case geometrize::shapes::ShapeTypes::ELLIPSE:
            return new Ellipse(xBound, yBound);
        case geometrize::shapes::ShapeTypes::ROTATED_ELLIPSE:
            return new RotatedEllipse(xBound, yBound);
        case geometrize::shapes::ShapeTypes::ROTATED_RECTANGLE:
            return new RotatedRectangle(xBound, yBound);
        case geometrize::shapes::ShapeTypes::TRIANGLE:
            return new Triangle(xBound, yBound);
        case geometrize::shapes::ShapeTypes::RECTANGLE:
            return new Rectangle(xBound, yBound);
        case geometrize::shapes::ShapeTypes::SHAPE_COUNT:
         assert(0 && "Bad shape value");
    };

    assert(0 && "Unhandled shape type encountered");
    return new Rectangle(xBound, yBound);
}

geometrize::Shape* randomShape(const std::uint32_t xBound, const std::uint32_t yBound)
{
    return create(geometrize::shapes::allShapes[commonutil::randomRange(0, static_cast<int>(geometrize::shapes::allShapes.size()) - 1)], xBound, yBound);
}

geometrize::Shape* randomShapeOf(const shapes::ShapeTypes t, const std::uint32_t xBound, const std::uint32_t yBound)
{
    return new Rectangle(xBound, yBound); // TODO

    const std::bitset<32> b(t);
    std::vector<std::uint32_t> bits;
    for(unsigned int bit = 0; bit < b.count(); bit++) {
        if(bit) {
            bits.push_back(1 << bit);
        }
    }
    return create(static_cast<geometrize::shapes::ShapeTypes>(bits[commonutil::randomRange(0, static_cast<std::int32_t>(bits.size()))]), xBound, yBound);
}

}
