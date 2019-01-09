#include "shapefactory.h"

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
#include "shapemutator.h"
#include "../commonutil.h"
#include "../rasterizer/rasterizer.h"

namespace geometrize
{

std::function<std::shared_ptr<geometrize::Shape>()> createDefaultShapeCreator(const geometrize::ShapeTypes types, const std::int32_t w, const std::int32_t h)
{
    auto f = [types, w, h]() {
        std::shared_ptr<geometrize::Shape> s = geometrize::randomShapeOf(types);

        switch(s->getType()) {
        case geometrize::ShapeTypes::RECTANGLE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Rectangle&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Rectangle&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Rectangle&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::ROTATED_RECTANGLE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::RotatedRectangle&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::RotatedRectangle&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::RotatedRectangle&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::TRIANGLE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Triangle&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Triangle&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Triangle&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::ELLIPSE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Ellipse&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Ellipse&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Ellipse&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::ROTATED_ELLIPSE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::RotatedEllipse&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::RotatedEllipse&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::RotatedEllipse&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::CIRCLE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Circle&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Circle&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Circle&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::LINE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Line&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Line&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Line&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::QUADRATIC_BEZIER: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::QuadraticBezier&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::QuadraticBezier&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::QuadraticBezier&>(s), w, h); };
            break;
        }
        case geometrize::ShapeTypes::POLYLINE: {
            s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Polyline&>(s), w, h); };
            s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Polyline&>(s), w, h); };
            s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Polyline&>(s), w, h); };
            break;
        }
        default:
            assert(0 && "Bad shape type");
        }

        return s;
    };

    return f;
}

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
