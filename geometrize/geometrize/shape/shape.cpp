#include "shape.h"

#include <string>

#include "../model.h"
#include "../rasterizer/rasterizer.h"

namespace geometrize
{

Shape::Shape() : m_model(nullptr)
{
}

Shape::Shape(const geometrize::Model& model) : m_model(&model)
{
}

const Model* Shape::getModel()
{
    return m_model;
}

const std::string Shape::SVG_STYLE_HOOK = "::svg_style_hook::";

bool shapesOverlap(const geometrize::Shape& a, const geometrize::Shape& b)
{
    return geometrize::scanlinesOverlap(a.rasterize(), b.rasterize());
}

bool shapeContains(const geometrize::Shape& container, const geometrize::Shape& containee)
{
    return geometrize::scanlinesContain(container.rasterize(), containee.rasterize());
}

}
