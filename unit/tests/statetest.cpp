#include "../catch.hpp"

#include "geometrize/state.h"
#include "geometrize/shape/rectangle.h"

TEST_CASE("Test state mutation", "[state]")
{
    geometrize::State state(geometrize::shapes::RECTANGLE, 128, 100, 100);
    geometrize::State oldState = state.mutate();
    REQUIRE(state.m_score == oldState.m_score);
}

TEST_CASE("Test state copying", "[state]")
{
    geometrize::State state(geometrize::shapes::RECTANGLE, 128, 100, 100);
    geometrize::State clone = state;
    REQUIRE(&state != &clone);
}
