#include "svgexporter.h"

#include <cstdint>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "../bitmap/rgba.h"
#include "../shape/shape.h"
#include "../shaperesult.h"
#include "../shape/rectangle.h"
#include "../commonutil.h"

namespace geometrize
{

namespace exporter
{

std::string getSVGRgbColorAttrib(const geometrize::rgba color)
{
    std::ostringstream stream;
    stream << "rgb("
           << static_cast<std::int32_t>(color.r) << ","
           << static_cast<std::int32_t>(color.g) << ","
           << static_cast<std::int32_t>(color.b) << ")";
    return stream.str();
}

std::string getSVGStrokeAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "stroke=\"" << getSVGRgbColorAttrib(color) << "\"";
    return stream.str();
}

std::string getSVGFillAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "fill=\"" << getSVGRgbColorAttrib(color) << "\"";
    return stream.str();
}

std::string getSVGFillOpacityAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "fill-opacity=\"" << static_cast<float>(color.a) / 255.0f << "\"";
    return stream.str();
}

std::string getSVGStrokeOpacityAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "stroke-opacity=\"" << static_cast<float>(color.a) / 255.0f << "\"";
    return stream.str();
}

std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height, const geometrize::rgba backgroundColor, const SVGExportOptions /*options*/)
{
    std::stringstream stream;

    stream << "<?xml version=\"1.0\" standalone=\"no\"?>" << "\n";
    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\" " << "width=\"" << width << "\" " << "height=\"" << height << "\">" << "\n";

    stream << "<rect "
           << "id=\"background\" "
           << "x=\"0\" y=\"0\" "
           << "width=\"" << width << "\" "
           << "height=\"" << height << "\" "
           << getSVGFillAttrib(backgroundColor) << " "
           << getSVGFillOpacityAttrib(backgroundColor)
           << "/>"
           << "\n";

    for(std::size_t i = 0; i < data.size(); i++) {
        const geometrize::ShapeResult& s(data[i]);
        std::string shapeData{s.shape->getSvgShapeData()};
        const geometrize::ShapeTypes shapeType{s.shape->getType()};

        std::string styles{""};

        styles.append("id=\"" + std::to_string(i) + "\" ");

        if(shapeType == geometrize::ShapeTypes::LINE
                || shapeType == geometrize::ShapeTypes::POLYLINE
                || shapeType == geometrize::ShapeTypes::QUADRATIC_BEZIER) {
            styles.append(getSVGStrokeAttrib(s.color));
            styles.append(" stroke-width=\"1\" fill=\"none\" ");
            styles.append(getSVGStrokeOpacityAttrib(s.color));
        } else {
            styles.append(getSVGFillAttrib(s.color));
            styles.append(" ");
            styles.append(getSVGFillOpacityAttrib(s.color));
        }

        shapeData = std::regex_replace(shapeData, std::regex(geometrize::Shape::SVG_STYLE_HOOK), styles);

        stream << shapeData << "\n";
    }

    stream << "</svg>";

    return stream.str();
}

}

}
