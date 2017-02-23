#include "shapearrayexporter.h"

#include <sstream>
#include <string>
#include <vector>

#include "../shaperesult.h"

namespace geometrize
{

namespace exporter
{

std::string exportShapeArray(const std::vector<geometrize::ShapeResult>& data)
{
    std::stringstream stream;

    for(int i = 0; i < data.size(); i++) {
        const geometrize::ShapeResult& s{data[i]};

        stream << s.shape->getType() << "\n";

        const std::vector<std::int32_t> shapeData{s.shape->getShapeData()};
        for(int d = 0; d < shapeData.size(); d++) {
            stream << shapeData[d];
            if(d <= shapeData.size() - 2) {
                stream << ",";
            }
        }
        stream << "\n";

        stream << static_cast<std::uint32_t>(s.color.r) << ","
               << static_cast<std::uint32_t>(s.color.g) << ","
               << static_cast<std::uint32_t>(s.color.b) << ","
               << static_cast<std::uint32_t>(s.color.a);

        if(i <= data.size() - 2) {
            stream << "\n";
        }
    }

    return stream.str();
}

}

}
