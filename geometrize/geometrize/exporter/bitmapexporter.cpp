#include "bitmapexporter.h"

#include <cstdint>
#include <sstream>

#include "../bitmap/bitmap.h"

namespace geometrize
{

namespace exporter
{

inline std::uint16_t flip16(const std::uint16_t& v)
{
    return (v >> 8) | (v << 8);
}

inline std::uint32_t flip32(const std::uint32_t& v)
{
    return (v & 0xFF000000) >> 0x18 | (v & 0x000000FF) << 0x18 | (v & 0x00FF0000) >> 0x08 | (v & 0x0000FF00) << 0x08;
}

inline bool bigEndian()
{
   std::uint32_t v{1};
   return (1 != reinterpret_cast<char*>(&v)[0]);
}

inline void writeToStream(std::ostringstream& stream, const std::uint32_t& t)
{
    if(bigEndian()) {
        const std::uint32_t flipped{flip32(t)};
        stream.write(reinterpret_cast<const char*>(&flipped), sizeof(std::uint32_t));
    } else {
        stream.write(reinterpret_cast<const char*>(&t), sizeof(std::uint32_t));
    }
}

inline void writeToStream(std::ostringstream& stream, const std::uint16_t& t)
{
    if(bigEndian()) {
        const std::uint16_t flipped{flip16(t)};
        stream.write(reinterpret_cast<const char*>(&flipped), sizeof(std::uint16_t));
    } else {
        stream.write(reinterpret_cast<const char*>(&t), sizeof(std::uint16_t));
    }
}

inline void writeToStream(std::ostringstream& stream, const std::uint8_t& t)
{
    stream.write(reinterpret_cast<const char*>(&t), sizeof(std::uint8_t));
}

std::string exportBMP(const geometrize::Bitmap& bitmapData)
{
    std::ostringstream stream(std::ios::binary);

    const std::uint32_t BITMAP_FILE_HEADER_SIZE{14U};
    const std::uint32_t BITMAP_INFORMATION_HEADER_SIZE{40U};

    const std::uint32_t width{bitmapData.getWidth()}; // The width of the image in pixels.
    const std::uint32_t height{bitmapData.getHeight()}; // The height of the image in pixels.

    // Per row pad byte count, used to ensure that each row is a multiple of 4 bytes.
    const std::uint32_t padding{ ((width * 3) % 4 != 0) ? 4 - ((width * 3) % 4) : 0};

    // Bitmap Information Header
    const std::uint32_t ifSize{BITMAP_INFORMATION_HEADER_SIZE}; // The number of bytes required by the structure.
    const std::uint16_t planes{1U}; // The number of planes for the target device. This value must be set to 1.
    const std::uint16_t bitCount{24U}; // The number of bits that define each pixel and the maximum number of colors in the bitmap.
    const std::uint32_t compression{0U}; // Specifies the bitmap compression type.
    const std::uint32_t imageSize{(((width * 3U) + padding) & 0x0000FFFC) * height}; // The size of the image in bytes.
    const std::uint32_t xPelsPerMeter{0U}; // The horizontal resolution in pixels-per-meter.
    const std::uint32_t yPelsPerMeter{0U}; // The vertical resolution in pixels-per-meter.
    const std::uint32_t colorsUsed{0U}; // The number of color indexes in the color table actually used by the bitmap.
    const std::uint32_t colorsImportant{0U}; // The number of color indexes required for displaying the bitmap. If zero, all colors are required.

    // Bitmap File Header
    const std::uint16_t type{19778U}; // The file type.
    const std::uint32_t fhSize{BITMAP_INFORMATION_HEADER_SIZE + BITMAP_FILE_HEADER_SIZE + imageSize}; // The size in bytes of the bitmap file.
    const std::uint16_t reserved1{0U}; // Reserved, must be zero.
    const std::uint16_t reserved2{0U}; // Reserved, must be zero.
    const std::uint32_t offbits{BITMAP_INFORMATION_HEADER_SIZE + BITMAP_FILE_HEADER_SIZE}; // The offset, in bytes, from the beginning of the BitmapFileHeader structure to the bitmap bits.

    writeToStream(stream, type);
    writeToStream(stream, fhSize);
    writeToStream(stream, reserved1);
    writeToStream(stream, reserved2);
    writeToStream(stream, offbits);

    writeToStream(stream, ifSize);
    writeToStream(stream, width);
    writeToStream(stream, height);
    writeToStream(stream, planes);
    writeToStream(stream, bitCount);
    writeToStream(stream, compression);
    writeToStream(stream, imageSize);
    writeToStream(stream, xPelsPerMeter);
    writeToStream(stream, yPelsPerMeter);
    writeToStream(stream, colorsUsed);
    writeToStream(stream, colorsImportant);

    // Bitmap Image Data
    for(std::uint32_t y = 0U; y < bitmapData.getHeight(); y++) {
        for(std::uint32_t x = 0U; x < bitmapData.getWidth(); x++) {
            const geometrize::rgba pixel(bitmapData.getPixel(x, y));
            writeToStream(stream, pixel.b);
            writeToStream(stream, pixel.g);
            writeToStream(stream, pixel.r);
        }
        for (std::uint32_t pad = 0U; pad < padding; pad++) {
            const std::uint8_t zeroPad{0U};
            writeToStream(stream, zeroPad);
        }
    }

    return stream.str();
}

}

}
