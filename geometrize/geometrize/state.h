#pragma once

#include <cstdint>
#include <memory>

namespace geometrize
{
class Bitmap;
class Model;
class Shape;
}

namespace geometrize
{

/**
 * @brief The State class relates a shape and related properties to a measure of how close it brings the working image closer to the target image.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class State
{
public:
    State(); // Seems to be necessary of because MSVC futures - "_Associated_state ctor assumes _Ty is default constructible"

    /**
     * @brief Creates a new state.
     * @param shape The shape.
     * @param alpha The color alpha of the geometric shape.
     */
    State(const std::shared_ptr<geometrize::Shape>& shape, std::uint8_t alpha);

    ~State() = default;
    State(const State& other);
    State& operator=(const State& other);

    /**
     * @brief Calculates a measure of the improvement drawing the primitive to the current bitmap will have.
     * The lower the energy, the better. The score is cached, set it to < 0 to recalculate it.
     * @return The energy measure.
     */
    float calculateEnergy(const geometrize::Bitmap& target, const geometrize::Bitmap& current, geometrize::Bitmap& buffer, float lastScore);

    /**
     * @brief mutate Modifies the current state in a random fashion.
     * @return The old state, useful for undoing the mutation or keeping track of previous states.
     */
    geometrize::State mutate();

    float m_score; ///< The score of the state, a measure of the improvement applying the state to the current bitmap will have.
    std::uint8_t m_alpha; ///< The alpha of the shape.
    std::shared_ptr<geometrize::Shape> m_shape; ///< The geometric primitive owned by the state.
};

}
