#include "svgexporter.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "../shape/shape.h"
#include "../shaperesult.h"

namespace geometrize
{

namespace exporter
{

std::string exportSVG(const std::vector<geometrize::ShapeResult>& data, const std::uint32_t width, const std::uint32_t height)
{
    std::stringstream stream;

    stream << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"" << width << "\" height=\"" << height << "\">" << "\n";

    // TODO width, height
    // TODO background rect?
    // TODO transformations (?)

    for(const geometrize::ShapeResult& s : data) {
        // TODO color
        // TODO opacity
        // TODO the actual shape data
        stream << s.shape->getSvgShapeData(); // TODO
    }

    stream << "</g>" << "\n";
    stream << "</svg>" << "\n";

    return stream.str();
}

}

}
