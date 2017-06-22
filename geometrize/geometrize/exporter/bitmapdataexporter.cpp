#include "bitmapdataexporter.h"

#include <cstdint>
#include <sstream>

#include "../bitmap/bitmap.h"

namespace geometrize
{

namespace exporter
{

inline void writeToStream(std::ostringstream& stream, const std::uint8_t& t)
{
    stream.write(reinterpret_cast<const char*>(&t), sizeof(std::uint8_t));
}

std::string exportBitmapData(const geometrize::Bitmap& bitmapData)
{
    std::ostringstream stream(std::ios::binary);

    for(std::uint32_t y = 0U; y < bitmapData.getHeight(); y++) {
        for(std::uint32_t x = 0U; x < bitmapData.getWidth(); x++) {
            const geometrize::rgba pixel(bitmapData.getPixel(x, y));
            writeToStream(stream, pixel.r);
            writeToStream(stream, pixel.g);
            writeToStream(stream, pixel.b);
            writeToStream(stream, pixel.a);
        }
    }

    return stream.str();
}

}

}
