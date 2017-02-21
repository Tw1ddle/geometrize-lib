#include "shapejsonexporter.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "../shaperesult.h"

namespace geometrize
{

namespace exporter
{

std::string exportShapeJson(const std::vector<geometrize::ShapeResult>& data)
{
    std::stringstream stream;
    stream << "{\n";

    for(const geometrize::ShapeResult& s : data) {
        const geometrize::shapes::ShapeTypes type{s.shape->getType()};
        const std::vector<std::int32_t> data{s.shape->getShapeData()};
        const geometrize::rgba color{s.color};

        stream << "\"shape\":" << "{\n";

        stream << "\"type\"" << type << ",\n";

        const std::int32_t iColor{(color.r << 24) + (color.g << 16) + (color.b << 8) + color.a};
        stream << "\"color\":" << iColor << "," << "\n";

        stream << "\"data\":" << "[";
        for(std::size_t i = 0; i < data.size(); i++) {
            stream << i;
            if(i != data.size()) {
                stream << ",";
            }
        }
        stream << "]";

        stream << "}\n";
    }

    stream << "}\n";
    return stream.str();
}

}

}
