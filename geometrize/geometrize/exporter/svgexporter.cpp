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

std::string getSVGRgbaColorAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "rgba("
           << static_cast<std::int32_t>(color.r) << ","
           << static_cast<std::int32_t>(color.g) << ","
           << static_cast<std::int32_t>(color.b) << ","
           << static_cast<float>(color.a) / 255.0f << ")";
    return stream.str();
}

std::string getSVGStrokeAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "stroke:" << getSVGRgbaColorAttrib(color);
    return stream.str();
}

std::string getSVGFillAttrib(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "fill:" << getSVGRgbaColorAttrib(color);
    return stream.str();
}

std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height, const geometrize::rgba backgroundColor, const SVGExportOptions options)
{
    std::stringstream stream;

    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" " << "width=\"" << width << "\" " << "height=\"" << height << "\">" << "\n";
    stream << "<g transform=\"translate(0.5 0.5)\">" << "\n";

    stream << "<rect "
           << "width=\"" << width << "\" "
           << "height=\"" << height << "\" "
           << "style=\"" << getSVGFillAttrib(backgroundColor) << "\""
           << "/>"
           << "\n";

    for(const geometrize::ShapeResult& s : data) {
        std::string shapeData{s.shape->getSvgShapeData()};
        const geometrize::shapes::ShapeTypes shapeType{s.shape->getType()};

        // TODO augment the shape data with color and other overrides specified in the export options

        std::string styles{"style=\""};

        if(shapeType == geometrize::shapes::ShapeTypes::LINE || shapeType == geometrize::shapes::ShapeTypes::POLYLINE) {
            styles.append(getSVGStrokeAttrib(s.color));
            styles.append(";stroke-width=1;fill:none");
        } else {
            styles.append(getSVGFillAttrib(s.color));
        }

        styles.append("\"");

        shapeData = std::regex_replace(shapeData, std::regex(geometrize::Shape::SVG_STYLE_HOOK), styles);

        stream << shapeData << "\n";
    }

    stream << "</g>" << "\n";
    stream << "</svg>" << "\n";

    return stream.str();
}

}

}
