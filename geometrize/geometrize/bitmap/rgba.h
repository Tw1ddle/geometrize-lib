#pragma once

#include <cstdint>

namespace geometrize
{

/**
 * @brief The rgba struct is a helper for manipulating RGBA8888 color data.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct rgba
{
    std::uint8_t r; ///> The red component (0-255).
    std::uint8_t g; ///> The green component (0-255).
    std::uint8_t b; ///> The blue component (0-255).
    std::uint8_t a; ///> The alpha component (0-255).
};

bool operator==(const geometrize::rgba& lhs, const geometrize::rgba& rhs);
bool operator!=(const geometrize::rgba& lhs, const geometrize::rgba& rhs);

}
