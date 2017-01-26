#pragma once

namespace geometrize
{

/**
 * @brief The rgba struct is a helper for manipulating color data.
 */
struct rgba
{
    unsigned char r; ///> The red component (0-255).
    unsigned char g; ///> The green component (0-255).
    unsigned char b; ///> The blue component (0-255).
    unsigned char a; ///> The alpha component (0-255).
};

inline bool operator==(const rgba& lhs, const rgba& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

inline bool operator!=(const rgba& lhs, const rgba& rhs)
{
    return !(lhs == rhs);
}

}
