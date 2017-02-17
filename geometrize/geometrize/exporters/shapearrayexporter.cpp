#include "shapearrayexporter.h"

#include <string>
#include <vector>

#include "../shaperesult.h"

namespace geometrize
{

namespace exporter
{

std::string exportShapeArray(const std::vector<geometrize::ShapeResult>& data)
{
    std::string result;

    for(const geometrize::ShapeResult& s : data) {
        // TODO write shape attribs, type and vertex positions
    }

    return result;
}

}

}
