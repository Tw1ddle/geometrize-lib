#pragma once

#include <cstdint>
#include <vector>

#include "rgba.h"

namespace geometrize
{

/**
 * Helper class for working with bitmap data.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class Bitmap
{
public:
    /**
     * @brief Bitmap Creates a new bitmap.
     * @param width The width of the bitmap.
     * @param height The height of the bitmap.
     * @param color The starting color of the bitmap (RGBA format).
     */
    inline Bitmap(const std::uint32_t width, const std::uint32_t height, const geometrize::rgba color) : m_width{width}, m_height{height}, m_data(width * height * 4)
    {
        for(std::size_t i = 0; i < m_data.size(); i += 4u) {
            m_data[i] = color.r;
            m_data[i + 1u] = color.g;
            m_data[i + 2u] = color.b;
            m_data[i + 3u] = color.a;
        }
    }

    /**
     * @brief Bitmap Creates a new bitmap from the supplied byte data.
     * @param width The width of the bitmap.
     * @param height The height of the bitmap.
     * @param data The byte data to fill the bitmap with, must be width * height * depth (4) long.
     */
    inline Bitmap(const std::uint32_t width, const std::uint32_t height, const std::vector<std::uint8_t>& data) : m_width{width}, m_height{height}, m_data{data} {}

    inline ~Bitmap() = default;
    inline Bitmap& operator=(const Bitmap&) = default;
    inline Bitmap(const Bitmap&) = default;

    /**
     * @brief getWidth Gets the width of the bitmap.
     */
    inline std::uint32_t getWidth() const
    {
        return m_width;
    }

    /**
     * @brief getHeight Gets the height of the bitmap.
     */
    inline std::uint32_t getHeight() const
    {
        return m_height;
    }

    /**
     * @brief copyData Gets a copy of the raw bitmap data.
     * @return	The bitmap data.
     */
    inline std::vector<std::uint8_t> copyData() const
    {
        return m_data;
    }

    /**
     * @brief getDataRef Gets a reference to the raw bitmap data.
     * @return  The bitmap data.
     */
    inline const std::vector<std::uint8_t>& getDataRef() const
    {
        return m_data;
    }

    /**
     * @brief getPixel Gets a pixel color value.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return The pixel RGBA color value.
     */
    inline geometrize::rgba getPixel(const std::uint32_t x, const std::uint32_t y) const
    {
        const std::size_t index{(m_width * y + x) * 4};
        return geometrize::rgba{m_data[index], m_data[index + 1], m_data[index + 2], m_data[index + 3]};
    }

    /**
     * @brief setPixel Sets a pixel color value.
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @param color The pixel RGBA color value.
     */
    inline void setPixel(const std::uint32_t x, const std::uint32_t y, const geometrize::rgba color)
    {
        const std::size_t index{(m_width * y + x) * 4};
        m_data[index] = color.r;
        m_data[index + 1u] = color.g;
        m_data[index + 2u] = color.b;
        m_data[index + 3u] = color.a;
    }

private:
    const std::uint32_t m_width; ///< The width of the bitmap.
    const std::uint32_t m_height; ///< The height of the bitmap.
    std::vector<std::uint8_t> m_data; ///< The bitmap data.
};

}
