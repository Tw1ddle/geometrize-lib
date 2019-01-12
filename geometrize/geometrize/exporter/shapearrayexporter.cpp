#include "shapearrayexporter.h"

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

std::string exportShapeArray(const std::vector<geometrize::ShapeResult>& data)
{
    std::ostringstream stream;

    for(std::size_t i = 0; i < data.size(); i++) {
        const geometrize::ShapeResult& s(data[i]);

        stream << static_cast<std::underlying_type<geometrize::ShapeTypes>::type>(s.shape->getType()) << "\n";

        const std::vector<float> shapeData{getRawShapeData(*s.shape.get())};
        for(std::size_t d = 0; d < shapeData.size(); d++) {
            stream << shapeData[d];
            if(d != (shapeData.size() - 1U)) {
                stream << ",";
            }
        }
        stream << "\n";

        stream << static_cast<std::uint32_t>(s.color.r) << ","
               << static_cast<std::uint32_t>(s.color.g) << ","
               << static_cast<std::uint32_t>(s.color.b) << ","
               << static_cast<std::uint32_t>(s.color.a);

        if(i != (data.size() - 1U)) {
            stream << "\n";
        }
    }

    return stream.str();
}

}

}
