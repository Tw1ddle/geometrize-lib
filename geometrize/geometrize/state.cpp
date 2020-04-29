#include "state.h"

#include <cassert>
#include <cstdint>
#include <vector>

#include "commonutil.h"
#include "bitmap/bitmap.h"
#include "core.h"
#include "model.h"
#include "shape/shape.h"
#include "rasterizer/scanline.h"

namespace geometrize
{

State::State() : m_score{-1.0f}, m_alpha{0}, m_shape{nullptr} {}

State::State(const std::shared_ptr<geometrize::Shape>& shape, const std::uint8_t alpha) :
    m_score{-1.0f}, m_alpha{alpha}, m_shape{shape}
{
    m_shape->setup(*m_shape);
}

State& State::operator=(const geometrize::State& other)
{
    if(this != &other) {
        m_score = other.m_score;
        m_alpha = other.m_alpha;
        m_shape = other.m_shape->clone();
    }
    return *this;
}

State::State(const geometrize::State& other) : m_score{other.m_score}, m_alpha{other.m_alpha}, m_shape{other.m_shape->clone()}
{
}

float State::calculateEnergy(const geometrize::Bitmap& target, const geometrize::Bitmap& current, geometrize::Bitmap& buffer, const float lastScore)
{
    assert(m_score < 0 && "Score was not reset");
    m_score = geometrize::core::energy(m_shape->rasterize(*m_shape), m_alpha, target, current, buffer, lastScore);
    return m_score;
}

geometrize::State State::mutate()
{
    geometrize::State oldState(*this);
    m_shape->mutate(*m_shape);
    m_score = -1;
    return oldState;
}

}
