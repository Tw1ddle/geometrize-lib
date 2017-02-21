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
 * @brief exportBitmapData Exports the raw image data to a binary dump - just the data in the given format, no zero-padding or anything.
 * @param bitmapData The image data to save as binary data.
 * @param filePath The full path to the binary dump file target (include the filename and optionally any extension).
 * @return True if the bitmap data was saved, else false.
 */
bool exportBitmapData(const geometrize::Bitmap& bitmapData, const std::string& filePath);

}

}
