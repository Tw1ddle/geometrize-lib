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
 * @brief exportBitmap Exports the image data to a RGB888 bitmap image file (BMP).
 * @param bitmapData The image data to save as a bitmap image file.
 * @param filePath The full path to the bitmap image file target (include the filename and .bmp extension).
 * @return True if the bitmap was saved, else false.
 */
bool exportBitmap(const geometrize::Bitmap& bitmapData, const std::string& filePath);

}

}
