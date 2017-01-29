#pragma once

#include <cstdint>
#include <vector>

#include "bitmap/bitmap.h"
#include "shape/shape.h"
#include "shape/shapefactory.h"
#include "shape/shapetypes.h"
#include "scanline.h"

namespace geometrize
{

namespace core
{
    // Forward declare energy function
    float energy(const std::vector<geometrize::Scanline>& lines,
                 const std::uint32_t alpha,
                 const geometrize::Bitmap& target,
                 const geometrize::Bitmap& current,
                 geometrize::Bitmap& buffer,
                 const float score);
}

/**
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class State
{
public:
    /**
     * @brief Creates a new state.
     * @param shapeTypes The types of geometric primitive to select.
     * @param alpha The color alpha of the geometric shape.
     * @param width The width of the bitmap.
     * @param height The height of the bitmap.
     */
    inline State(const shapes::ShapeTypes shapeTypes, const std::uint32_t alpha, const std::uint32_t width, const std::uint32_t height) :
        m_score{-1.0f}, m_alpha{alpha}, m_shape{ShapeFactory::randomShapeOf(shapeTypes, width, height)}
    {}

    inline ~State() = default;
    inline geometrize::State& operator=(const geometrize::State& other)
    {
        if(this != &other) {
            m_shape = other.m_shape->clone();
            m_score = other.m_score;
            m_alpha = other.m_alpha;
        }
        return *this;
    }

    inline geometrize::State(const geometrize::State& other)
    {
        m_shape = other.m_shape->clone();
        m_score = other.m_score;
        m_alpha = other.m_alpha;
    }

    /**
     * @brief Calculates a measure of the improvement drawing the primitive to the current bitmap will have.
     * The lower the energy, the better. The score is cached, set it to < 0 to recalculate it.
     * @return The energy measure.
     */
    inline float calculateEnergy(const geometrize::Bitmap& target, const geometrize::Bitmap& current, geometrize::Bitmap& buffer)
    {
        if(m_score < 0) {
            m_score = geometrize::core::energy(m_shape->rasterize(), m_alpha, target, current, buffer, m_score);
        }
        return m_score;
    }

    /**
     * @brief mutate Modifies the current state in a random fashion.
     * @return The old state - in case we want to go back to the old state.
     */
    inline geometrize::State mutate()
    {
        geometrize::State oldState(*this);
        oldState.m_score = -1;
        m_shape->mutate();
        m_score = -1;
        return oldState;
    }

    float m_score; ///< The score of the state, a measure of the improvement applying the state to the current bitmap will have.
    std::uint32_t m_alpha; ///< The alpha of the shape.
    geometrize::Shape* m_shape; ///< The geometric primitive owned by the state. // TODO watch memory leaks!!!!!!!!!!!!!!!!!!!!!!!!!!
};

}
