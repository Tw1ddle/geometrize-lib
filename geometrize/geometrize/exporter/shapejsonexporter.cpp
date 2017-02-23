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

    for(std::size_t i = 0; i < data.size(); i++) {
        const geometrize::ShapeResult& s{data[i]};

        stream << "\"shape_" << i << "\":" << "\n{\n";

        const geometrize::shapes::ShapeTypes type{s.shape->getType()};
        stream << "\"type\":" << type << ",\n";

        const geometrize::rgba color{s.color};
        const std::int32_t colorValue{(color.r << 24) + (color.g << 16) + (color.b << 8) + color.a};
        stream << "\"color\":" << static_cast<std::uint32_t>(colorValue) << "," << "\n";

        const std::vector<std::int32_t> shapeData{s.shape->getShapeData()};
        stream << "\"data\":" << "[";
        for(std::size_t d = 0; d < shapeData.size(); d++) {
            stream << shapeData[d];
            if(d <= shapeData.size() - 2) {
                stream << ",";
            }
        }
        stream << "]";
        stream << "\n}";

        if(i <= data.size() - 2) {
            stream << ",\n";
        }
    }

    stream << "\n}";
    return stream.str();
}

}

}
