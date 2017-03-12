#include "svgexporter.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "../bitmap/rgba.h"
#include "../shape/shape.h"
#include "../shaperesult.h"
#include "../shape/rectangle.h"

namespace geometrize
{

namespace exporter
{

std::string getSVGAttribs(const geometrize::rgba color)
{
    std::stringstream stream;
    stream << "style=\""
           << "fill:rgba("
           << static_cast<std::int32_t>(color.r) << ","
           << static_cast<std::int32_t>(color.g) << ","
           << static_cast<std::int32_t>(color.b) << ","
           << static_cast<float>(color.a) / 255.0f << ")"
           << "\"";

    return stream.str();
}

// TODO add SVG exporter options (that will be used by getSvgShapeData function)
std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height, const geometrize::rgba backgroundColor)
{
    std::stringstream stream;

    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" " << "width=\"" << width << "\" " << "height=\"" << height << "\">" << "\n";
    stream << "<g transform=\"translate(0.5 0.5)\">" << "\n";

    // Add a background rect first
    stream << "<rect "
           << "width=\"" << width << "\" "
           << "height=\"" << height << "\" "
           << getSVGAttribs(backgroundColor)
           << "/>"
           << "\n";

    for(const geometrize::ShapeResult& s : data) {
        stream << s.shape->getSvgShapeData(getSVGAttribs(s.color)) << "\n";
    }

    stream << "</g>" << "\n";
    stream << "</svg>" << "\n";

    return stream.str();
}

}

}
