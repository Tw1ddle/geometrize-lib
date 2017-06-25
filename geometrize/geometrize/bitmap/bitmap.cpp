#include "bitmap.h"

#include <cassert>

#include "rgba.h"

namespace geometrize
{

Bitmap::Bitmap(const std::uint32_t width, const std::uint32_t height, const geometrize::rgba color) : m_width{width}, m_height{height}, m_data(width * height * 4U)
{
    fill(color);
}

Bitmap::Bitmap(const std::uint32_t width, const std::uint32_t height, const std::vector<std::uint8_t>& data) : m_width{width}, m_height{height}, m_data{data}
{
    assert((width * height * 4U) == data.size());
}

std::uint32_t Bitmap::getWidth() const
{
    return m_width;
}

std::uint32_t Bitmap::getHeight() const
{
    return m_height;
}

std::vector<std::uint8_t> Bitmap::copyData() const
{
    return m_data;
}

const std::vector<std::uint8_t>& Bitmap::getDataRef() const
{
    return m_data;
}

geometrize::rgba Bitmap::getPixel(const std::uint32_t x, const std::uint32_t y) const
{
    const std::size_t index{(m_width * y + x) * 4U};
    return geometrize::rgba{m_data[index], m_data[index + 1U], m_data[index + 2U], m_data[index + 3U]};
}

void Bitmap::setPixel(const std::uint32_t x, const std::uint32_t y, const geometrize::rgba color)
{
    const std::uint32_t index{(m_width * y + x) * 4U};
    m_data[index] = color.r;
    m_data[index + 1U] = color.g;
    m_data[index + 2U] = color.b;
    m_data[index + 3U] = color.a;
}

void Bitmap::fill(const geometrize::rgba color)
{
    for(std::size_t i = 0; i < m_data.size(); i += 4U) {
        m_data[i] = color.r;
        m_data[i + 1U] = color.g;
        m_data[i + 2U] = color.b;
        m_data[i + 3U] = color.a;
    }
}

}
