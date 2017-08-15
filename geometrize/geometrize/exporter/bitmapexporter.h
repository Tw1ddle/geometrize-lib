#pragma once

#include <string>

namespace geometrize
{
class Bitmap;
}

namespace geometrize
{

namespace exporter
{

/**
 * @brief exportBMP Exports the image data to a RGB888 bitmap image file (BMP).
 * @param bitmapData The image data to save as a bitmap image file.
 * @return A string containing the bitmap data.
 */
std::string exportBMP(const geometrize::Bitmap& bitmapData);

}

}
