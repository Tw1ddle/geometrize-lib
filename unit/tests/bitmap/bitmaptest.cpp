#include "../catch.hpp"

#include <vector>

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"

/**
 * @brief bitmapIsColor Checks if all the pixels in the bitmap are a given color.
 * @param bitmap The bitmap to check.
 * @param color The color to check for.
 * @return True if all the pixels in the bitmap are the given color.
 */
bool bitmapIsColor(const geometrize::Bitmap& bitmap, const geometrize::rgba color)
{
    for(unsigned int x = 0; x < bitmap.getWidth(); x++) {
        for(unsigned int y = 0; y < bitmap.getHeight(); y++) {
            auto z = bitmap.getPixel(x, y);
            if(bitmap.getPixel(x, y) != color) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief makeDataBitmap Creates a test data bitmap.
 * @param width The width of the bitmap.
 * @param height The height of the bitmap.
 * @param color The color of the pixels of the bitmap.
 * @return The new bitmap.
 */
geometrize::Bitmap makeDataBitmap(const int width, const int height, geometrize::rgba color = { 255, 0, 0, 255 })
{
    std::vector<unsigned char> data;
    for(int i = 0; i < width * height * 4; i+=4) {
        data.push_back(color.r);
        data.push_back(color.g);
        data.push_back(color.b);
        data.push_back(color.a);
    }
    return geometrize::Bitmap(width, height, data);
}

TEST_CASE("Bitmaps can be created programmatically", "[bitmap]")
{
    SECTION("A 20x100 red square bitmap")
    {
        const int width{20};
        const int height{100};
        geometrize::rgba red{255, 0, 0, 255};
        geometrize::Bitmap bitmap(width, height, red);

        REQUIRE(bitmap.getWidth() == width);
        REQUIRE(bitmap.getHeight() == height);
        REQUIRE(bitmapIsColor(bitmap, red));
    }

    SECTION("A 5x5 red square bitmap from individual pixel values data")
    {
        const int width{5};
        const int height{5};
        geometrize::rgba red{255, 0, 0, 255};

        geometrize::Bitmap bitmap(makeDataBitmap(width, height));
        REQUIRE(bitmap.getWidth() == width);
        REQUIRE(bitmap.getHeight() == height);
        REQUIRE(bitmapIsColor(bitmap, red));
    }

    SECTION("Non-rectangular bitmaps are supported")
    {
        const int width{44};
        const int height{99};
        geometrize::Bitmap bitmap(makeDataBitmap(width, height));
        REQUIRE(bitmap.getWidth() == width);
        REQUIRE(bitmap.getHeight() == height);
    }
}

TEST_CASE("Bitmap data can be deep-copied", "[bitmap]")
{
    SECTION("Clone data")
    {
        const geometrize::Bitmap bitmap(100, 100, geometrize::rgba{255, 0, 0, 255});
        std::vector<unsigned char> data{bitmap.copyData()};
        REQUIRE(bitmap.getDataRef() == data);
    }
}

TEST_CASE("Bitmap pixel colors can be retrieved", "[bitmap]")
{
    SECTION("Simple get pixel")
    {
        geometrize::rgba red{255, 0, 0, 255};
        REQUIRE(red == makeDataBitmap(50, 50).getPixel(0, 0));
        REQUIRE(red != makeDataBitmap(50, 50, geometrize::rgba{0, 255, 255, 255}).getPixel(0, 0));
    }
}

TEST_CASE("Bitmap pixel colors can be set", "[bitmap]")
{
    SECTION("Simple set pixel")
    {
        geometrize::rgba red{255, 0, 0, 255};
        geometrize::rgba blue{0, 0, 255, 255};
        geometrize::Bitmap bitmap(100, 100, red);
        bitmap.setPixel(0, 0, blue);
        REQUIRE(blue == bitmap.getPixel(0, 0));
        REQUIRE(red == bitmap.getPixel(1, 0));
        REQUIRE(red == bitmap.getPixel(0, 1));
    }
}
