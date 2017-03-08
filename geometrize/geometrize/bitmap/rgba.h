#pragma once

#include <cstdint>

namespace geometrize
{

/**
 * @brief The rgba struct is a helper for manipulating color data.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
struct rgba
{
    std::uint8_t r; ///> The red component (0-255).
    std::uint8_t g; ///> The green component (0-255).
    std::uint8_t b; ///> The blue component (0-255).
    std::uint8_t a; ///> The alpha component (0-255).
};

inline bool operator==(const geometrize::rgba& lhs, const geometrize::rgba& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

inline bool operator!=(const geometrize::rgba& lhs, const geometrize::rgba& rhs)
{
    return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b || lhs.a != rhs.a;
}

}
