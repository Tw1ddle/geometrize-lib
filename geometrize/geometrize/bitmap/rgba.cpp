#include "rgba.h"

namespace geometrize
{

bool operator==(const geometrize::rgba& lhs, const geometrize::rgba& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

bool operator!=(const geometrize::rgba& lhs, const geometrize::rgba& rhs)
{
    return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b || lhs.a != rhs.a;
}

}
