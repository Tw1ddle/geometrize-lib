#include "shape.h"

#include <string>

#include "../model.h"

namespace geometrize
{

Shape::Shape(const geometrize::Model& model) : m_model{model}
{
}

const Model& Shape::getModel()
{
    return m_model;
}

const std::string Shape::SVG_STYLE_HOOK = "::svg_style_hook::";

}
