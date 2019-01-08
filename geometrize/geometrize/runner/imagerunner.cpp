#include "imagerunner.h"

#include <cassert>
#include <memory>
#include <vector>

#include "../bitmap/bitmap.h"
#include "../core.h"
#include "../model.h"
#include "../rasterizer/rasterizer.h"
#include "../shape/shape.h"
#include "../shape/rectangle.h"
#include "../shape/rotatedrectangle.h"
#include "../shape/triangle.h"
#include "../shape/ellipse.h"
#include "../shape/rotatedellipse.h"
#include "../shape/circle.h"
#include "../shape/line.h"
#include "../shape/quadraticbezier.h"
#include "../shape/polyline.h"
#include "../shape/shapefactory.h"
#include "../shape/shapemutator.h"
#include "../shape/shapetypes.h"
#include "imagerunneroptions.h"

namespace geometrize
{

class ImageRunner::ImageRunnerImpl
{
public:
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap) : m_model{targetBitmap} {}
    ImageRunnerImpl(const geometrize::Bitmap& targetBitmap, const geometrize::Bitmap& initialBitmap) : m_model{targetBitmap, initialBitmap} {}
    ~ImageRunnerImpl() = default;
    ImageRunnerImpl& operator=(const ImageRunnerImpl&) = delete;
    ImageRunnerImpl(const ImageRunnerImpl&) = delete;

    std::vector<geometrize::ShapeResult> step(const geometrize::ImageRunnerOptions& options)
    {
        // TODO get rid of all the shared ptrs and just copy shapes by value?
        const std::int32_t w = m_model.getTarget().getWidth();
        const std::int32_t h = m_model.getTarget().getHeight();
        const geometrize::ShapeTypes types = options.shapeTypes;

        auto shapeCreator = [types, w, h]() {
            std::shared_ptr<geometrize::Shape> s = geometrize::randomShapeOf(types);

            switch(s->getType()) {
            case geometrize::ShapeTypes::RECTANGLE: {
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Rectangle&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Rectangle&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Rectangle&>(s), w, h); };
            } break;
            case geometrize::ShapeTypes::ROTATED_RECTANGLE:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::RotatedRectangle&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::RotatedRectangle&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::RotatedRectangle&>(s), w, h); };
                break;
            case geometrize::ShapeTypes::TRIANGLE:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Triangle&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Triangle&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Triangle&>(s), w, h); };
                break;
            case geometrize::ShapeTypes::ELLIPSE:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Ellipse&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Ellipse&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Ellipse&>(s), w, h); };
                break;
            case geometrize::ShapeTypes::ROTATED_ELLIPSE:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::RotatedEllipse&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::RotatedEllipse&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::RotatedEllipse&>(s), w, h); };
                break;
            case geometrize::ShapeTypes::CIRCLE:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Circle&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Circle&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Circle&>(s), w, h); };
                break;
            case geometrize::ShapeTypes::LINE:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Line&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Line&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Line&>(s), w, h); };
                break;
            case geometrize::ShapeTypes::QUADRATIC_BEZIER:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::QuadraticBezier&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::QuadraticBezier&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::QuadraticBezier&>(s), w, h); };
                break;
            case geometrize::ShapeTypes::POLYLINE:
                s->setup = [w, h](geometrize::Shape& s) { return geometrize::setup(static_cast<geometrize::Polyline&>(s), w, h); };
                s->mutate = [w, h](geometrize::Shape& s) { geometrize::mutate(static_cast<geometrize::Polyline&>(s), w, h); };
                s->rasterize = [w, h](const geometrize::Shape& s) { return geometrize::rasterize(static_cast<const geometrize::Polyline&>(s), w, h); };
                break;
            default:
                assert(0 && "Bad shape type");
            }

            s->setup(*s);
            return s;
        };

        m_model.setSeed(options.seed);
        return m_model.step(shapeCreator, options.alpha, options.shapeCount, options.maxShapeMutations, options.maxThreads);
    }

    geometrize::Bitmap& getCurrent()
    {
        return m_model.getCurrent();
    }

    geometrize::Bitmap& getTarget()
    {
        return m_model.getTarget();
    }

    const geometrize::Bitmap& getCurrent() const
    {
        return m_model.getCurrent();
    }

    const geometrize::Bitmap& getTarget() const
    {
        return m_model.getTarget();
    }

    geometrize::Model& getModel()
    {
        return m_model;
    }

private:
    geometrize::Model m_model; ///< The model for the primitive optimization/fitting algorithm.
};

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap) :
    d{std::unique_ptr<ImageRunner::ImageRunnerImpl>(new ImageRunner::ImageRunnerImpl(targetBitmap))}
{}

ImageRunner::ImageRunner(const geometrize::Bitmap& targetBitmap,  const geometrize::Bitmap& initialBitmap) :
    d{std::unique_ptr<ImageRunner::ImageRunnerImpl>(new ImageRunner::ImageRunnerImpl(targetBitmap, initialBitmap))}
{}

ImageRunner::~ImageRunner()
{}

std::vector<geometrize::ShapeResult> ImageRunner::step(const geometrize::ImageRunnerOptions& options)
{
    return d->step(options);
}

geometrize::Bitmap& ImageRunner::getCurrent()
{
    return d->getCurrent();
}

geometrize::Bitmap& ImageRunner::getTarget()
{
    return d->getTarget();
}

const geometrize::Bitmap& ImageRunner::getCurrent() const
{
    return d->getCurrent();
}

const geometrize::Bitmap& ImageRunner::getTarget() const
{
    return d->getTarget();
}

geometrize::Model& ImageRunner::getModel()
{
    return d->getModel();
}

}
