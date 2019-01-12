#include "shapejsonexporter.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "shapeserializer.h"
#include "../shape/shape.h"
#include "../shape/shapetypes.h"
#include "../shaperesult.h"

namespace geometrize
{

namespace exporter
{

std::string exportShapeJson(const std::vector<geometrize::ShapeResult>& data)
{
    std::ostringstream stream;
    stream << "{\"shapes\":\n[";

    for(std::size_t i = 0; i < data.size(); i++) {
        const geometrize::ShapeResult& s(data[i]);
        const geometrize::ShapeTypes type{s.shape->getType()};
        const std::vector<float> shapeData{getRawShapeData(*s.shape.get())};
        const geometrize::rgba color(s.color);
        const float score{s.score};

        stream << "{" << "\"type\":" << static_cast<std::underlying_type<geometrize::ShapeTypes>::type>(type) << ", \"data\":[";
        for(std::size_t d = 0; d < shapeData.size(); d++) {
            stream << shapeData[d];
            if(d <= shapeData.size() - 2) {
                stream << ",";
            }
        }
        stream << "],\"color\":[" << static_cast<std::uint32_t>(color.r) << "," << static_cast<std::uint32_t>(color.g) << "," << static_cast<std::uint32_t>(color.b) << "," << static_cast<std::uint32_t>(color.a) << "],";
        stream << "\"score\":" << score << "}";

        if(i <= data.size() - 2) {
            stream << ",\n";
        }
    }

    stream << "\n]}";
    return stream.str();
}

}

}
