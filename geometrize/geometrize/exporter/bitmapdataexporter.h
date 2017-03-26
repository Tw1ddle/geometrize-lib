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
 * @brief exportBitmapData Exports the raw image data to a binary dump - just the data as RGBA8888, no zero-padding or anything.
 * @param bitmapData The image data to save as binary data.
 * @return A string containing the raw bitmap data.
 */
std::string exportBitmapData(const geometrize::Bitmap& bitmapData);

}

}
