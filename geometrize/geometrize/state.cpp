#include "state.h"

#include <cstdint>
#include <memory>

#include "shape/shape.h"

namespace geometrize
{

State::State() : m_score{-1.0}, m_alpha{0}, m_shape{nullptr} {}

State::State(const std::shared_ptr<geometrize::Shape>& shape, const std::uint8_t alpha) :
    m_score{-1.0}, m_alpha{alpha}, m_shape{shape}
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

geometrize::State State::mutate()
{
    geometrize::State oldState(*this);
    m_shape->mutate(*m_shape);
    m_score = -1;
    return oldState;
}

}
