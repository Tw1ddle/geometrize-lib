#include "rasterizer.h"

#include <cstdint>
#include <vector>

#include "../bitmap/bitmap.h"
#include "../bitmap/rgba.h"
#include "scanline.h"

namespace geometrize
{

void drawLines(geometrize::Bitmap& image, const geometrize::rgba color, const std::vector<geometrize::Scanline>& lines)
{
    // Convert the non-premultiplied color to alpha-premultiplied 16-bits per channel RGBA
    // In other words, scale the rgb color components by the alpha component
    std::uint32_t sr{color.r};
    sr |= sr << 8;
    sr *= color.a;
    sr /= UINT8_MAX;
    std::uint32_t sg{color.g};
    sg |= sg << 8;
    sg *= color.a;
    sg /= UINT8_MAX;
    std::uint32_t sb{color.b};
    sb |= sb << 8;
    sb *= color.a;
    sb /= UINT8_MAX;
    std::uint32_t sa{color.a};
    sa |= sa << 8;

    const std::uint32_t m{UINT16_MAX};
    const std::uint32_t aa{(m - sa) * 257U};

    for(const geometrize::Scanline& line : lines) {
        const std::int32_t y{line.y};

        for(std::int32_t x = line.x1; x <= line.x2; x++) {
            const geometrize::rgba d(image.getPixel(x, y));

            const std::uint8_t r{static_cast<std::uint8_t>(((d.r * aa + sr * m) / m) >> 8)};
            const std::uint8_t g{static_cast<std::uint8_t>(((d.g * aa + sg * m) / m) >> 8)};
            const std::uint8_t b{static_cast<std::uint8_t>(((d.b * aa + sb * m) / m) >> 8)};
            const std::uint8_t a{static_cast<std::uint8_t>(((d.a * aa + sa * m) / m) >> 8)};

            image.setPixel(x, y, geometrize::rgba{r, g, b, a});
        }
    }
}

void copyLines(geometrize::Bitmap& destination, const geometrize::Bitmap& source, const std::vector<geometrize::Scanline>& lines)
{
    for(const geometrize::Scanline& line : lines) {
        const std::int32_t y{line.y};
        for(std::int32_t x = line.x1; x < line.x2; x++) {
            destination.setPixel(x, y, source.getPixel(x, y));
        }
    }
}

}
