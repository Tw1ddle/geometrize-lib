#pragma once

#include <cstdint>
#include <memory>

#include "shape/shapetypes.h"

namespace geometrize
{
class Bitmap;
class Model;
class Shape;
}

namespace geometrize
{

/**
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class State
{
public:
    State(); // Seems to be necessary of because MSVC futures - "_Associated_state ctor assumes _Ty is default constructible"

    /**
     * @brief Creates a new state.
     * @param model The model that created this state.
     * @param shapeTypes The types of geometric primitive to select.
     * @param alpha The color alpha of the geometric shape.
     * @param width The width of the bitmap.
     * @param height The height of the bitmap.
     */
    State(const geometrize::Model& model, shapes::ShapeTypes shapeTypes, std::uint32_t alpha, std::uint32_t width, std::uint32_t height);

    ~State() = default;
    State(const State& other);
    State& operator=(const State& other);

    /**
     * @brief Calculates a measure of the improvement drawing the primitive to the current bitmap will have.
     * The lower the energy, the better. The score is cached, set it to < 0 to recalculate it.
     * @return The energy measure.
     */
    float calculateEnergy(const geometrize::Bitmap& target, const geometrize::Bitmap& current, geometrize::Bitmap& buffer);

    /**
     * @brief mutate Modifies the current state in a random fashion.
     * @return The old state - in case we want to go back to the old state.
     */
    geometrize::State mutate();

    float m_score; ///< The score of the state, a measure of the improvement applying the state to the current bitmap will have.
    std::uint32_t m_alpha; ///< The alpha of the shape.
    std::shared_ptr<geometrize::Shape> m_shape; ///< The geometric primitive owned by the state.
};

}
