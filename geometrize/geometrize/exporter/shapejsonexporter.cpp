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

        stream << "    \"shape_" << i << "\":" << " {\n";

        const geometrize::shapes::ShapeTypes type{s.shape->getType()};
        stream << "        \"type\":" << type << ",\n";

        const std::vector<std::int32_t> shapeData{s.shape->getRawShapeData()};
        stream << "        \"data\":" << "[";
        for(std::size_t d = 0; d < shapeData.size(); d++) {
            stream << shapeData[d];
            if(d <= shapeData.size() - 2) {
                stream << ",";
            }
        }
        stream << "],\n";

        const geometrize::rgba color{s.color};
        stream << "        \"color\":" << "[";
        stream << static_cast<std::uint32_t>(color.r) << ",";
        stream << static_cast<std::uint32_t>(color.g) << ",";
        stream << static_cast<std::uint32_t>(color.b) << ",";
        stream << static_cast<std::uint32_t>(color.a);
        stream << "],\n";

        stream << "        \"score\":" << s.score << "\n";

        stream << "    }";

        if(i <= data.size() - 2) {
            stream << ",\n";
        }
    }

    stream << "\n}";
    return stream.str();
}

}

}
