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

std::shared_ptr<geometrize::Shape> create(geometrize::shapes::ShapeTypes t, const std::uint32_t xBound, const std::uint32_t yBound)
{
    switch(t) {
        case geometrize::shapes::ShapeTypes::CIRCLE:
            return std::make_shared<geometrize::Circle>(xBound, yBound);
        case geometrize::shapes::ShapeTypes::ELLIPSE:
            return std::make_shared<geometrize::Ellipse>(xBound, yBound);
        case geometrize::shapes::ShapeTypes::ROTATED_ELLIPSE:
            return std::make_shared<geometrize::RotatedEllipse>(xBound, yBound);
        case geometrize::shapes::ShapeTypes::ROTATED_RECTANGLE:
            return std::make_shared<geometrize::RotatedRectangle>(xBound, yBound);
        case geometrize::shapes::ShapeTypes::TRIANGLE:
            return std::make_shared<geometrize::Triangle>(xBound, yBound);
        case geometrize::shapes::ShapeTypes::RECTANGLE:
            return std::make_shared<geometrize::Rectangle>(xBound, yBound);
        case geometrize::shapes::ShapeTypes::SHAPE_COUNT:
         assert(0 && "Bad shape value");
    };

    assert(0 && "Unhandled shape type encountered");
    return std::make_shared<geometrize::Rectangle>(xBound, yBound);
}

std::shared_ptr<geometrize::Shape> randomShape(const std::uint32_t xBound, const std::uint32_t yBound)
{
    return create(geometrize::shapes::allShapes[commonutil::randomRange(0, static_cast<int>(geometrize::shapes::allShapes.size()) - 1)], xBound, yBound);
}

std::shared_ptr<geometrize::Shape>  randomShapeOf(const shapes::ShapeTypes t, const std::uint32_t xBound, const std::uint32_t yBound)
{
    return std::make_shared<geometrize::Rectangle>(xBound, yBound); // TODO

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
