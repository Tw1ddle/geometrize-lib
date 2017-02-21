#include "bitmapdataexporter.h"

#include <cstdint>
#include <fstream>

#include "../bitmap/bitmap.h"

namespace geometrize
{

namespace exporter
{

inline void writeToStream(std::ofstream& stream, const std::uint8_t& t)
{
    stream.write(reinterpret_cast<const char*>(&t), sizeof(std::uint8_t));
}

bool exportBitmapData(const geometrize::Bitmap& bitmapData, const std::string& filePath)
{
    std::ofstream stream(filePath.c_str(), std::ios::binary);
    if(!stream) {
        return false;
    }

    // Bitmap Image Data
    for(std::uint32_t y = 0U; y < bitmapData.getHeight(); y++) {
        for(std::uint32_t x = 0U; x < bitmapData.getWidth(); x++) {
            const geometrize::rgba pixel{bitmapData.getPixel(x, y)};
            writeToStream(stream, pixel.r);
            writeToStream(stream, pixel.g);
            writeToStream(stream, pixel.b);
            writeToStream(stream, pixel.a);
        }
    }

    stream.close();

    return true;
}

}

}
