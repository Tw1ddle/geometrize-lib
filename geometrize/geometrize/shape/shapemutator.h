#pragma once

#include <functional>

namespace geometrize
{
class Circle;
class Ellipse;
class Line;
class Polyline;
class QuadraticBezier;
class Rectangle;
class RotatedEllipse;
class RotatedRectangle;
class Triangle;
}

namespace geometrize
{

// Default implementations that perform initial setup on each type of shape
void setupCircle(geometrize::Circle& shape);
void setupEllipse(geometrize::Ellipse& shape);
void setupLine(geometrize::Line& shape);
void setupPolyline(geometrize::Polyline& shape);
void setupQuadraticBezier(geometrize::QuadraticBezier& shape);
void setupRectangle(geometrize::Rectangle& shape);
void setupRotatedEllipse(geometrize::RotatedEllipse& shape);
void setupRotatedRectangle(geometrize::RotatedRectangle& shape);
void setupTriangle(geometrize::Triangle& shape);

// Default implementations that mutate each type of shape
void mutateCircle(geometrize::Circle& shape);
void mutateEllipse(geometrize::Ellipse& shape);
void mutateLine(geometrize::Line& shape);
void mutatePolyline(geometrize::Polyline& shape);
void mutateQuadraticBezier(geometrize::QuadraticBezier& shape);
void mutateRectangle(geometrize::Rectangle& shape);
void mutateRotatedEllipse(geometrize::RotatedEllipse& shape);
void mutateRotatedRectangle(geometrize::RotatedRectangle& shape);
void mutateTriangle(geometrize::Triangle& shape);

/**
 * @brief The ShapeMutator class is responsible for setting up and mutating shapes. It lets clients bind their own setup/mutation functions for specific shape types.
 * @author Sam Twidale (http://samcodes.co.uk/)
 */
class ShapeMutator
{
public:
    ShapeMutator();
    ~ShapeMutator() = default;
    ShapeMutator& operator=(const ShapeMutator&) = delete;
    ShapeMutator(const ShapeMutator&) = delete;

    /**
     * @brief setDefaults Sets all of the setup and mutation function handles to the default implementations.
     */
    void setDefaults();

    void setup(geometrize::Circle& shape) const;
    void mutate(geometrize::Circle& shape) const;
    void setSetupFunction(const std::function<void(geometrize::Circle&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::Circle&)>& f);

    void setup(geometrize::Ellipse& shape) const;
    void mutate(geometrize::Ellipse& shape) const;
    void setSetupFunction(const std::function<void(geometrize::Ellipse&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::Ellipse&)>& f);

    void setup(geometrize::Line& shape) const;
    void mutate(geometrize::Line& shape) const;
    void setSetupFunction(const std::function<void(geometrize::Line&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::Line&)>& f);

    void setup(geometrize::Polyline& shape) const;
    void mutate(geometrize::Polyline& shape) const;
    void setSetupFunction(const std::function<void(geometrize::Polyline&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::Polyline&)>& f);

    void setup(geometrize::QuadraticBezier& shape) const;
    void mutate(geometrize::QuadraticBezier& shape) const;
    void setSetupFunction(const std::function<void(geometrize::QuadraticBezier&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::QuadraticBezier&)>& f);

    void setup(geometrize::Rectangle& shape) const;
    void mutate(geometrize::Rectangle& shape) const;
    void setSetupFunction(const std::function<void(geometrize::Rectangle&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::Rectangle&)>& f);

    void setup(geometrize::RotatedEllipse& shape) const;
    void mutate(geometrize::RotatedEllipse& shape) const;
    void setSetupFunction(const std::function<void(geometrize::RotatedEllipse&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::RotatedEllipse&)>& f);

    void setup(geometrize::RotatedRectangle& shape) const;
    void mutate(geometrize::RotatedRectangle& shape) const;
    void setSetupFunction(const std::function<void(geometrize::RotatedRectangle&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::RotatedRectangle&)>& f);

    void setup(geometrize::Triangle& shape) const;
    void mutate(geometrize::Triangle& shape) const;
    void setSetupFunction(const std::function<void(geometrize::Triangle&)>& f);
    void setMutatorFunction(const std::function<void(geometrize::Triangle&)>& f);

private:
    std::function<void(geometrize::Circle&)> m_setupCircle;
    std::function<void(geometrize::Ellipse&)> m_setupEllipse;
    std::function<void(geometrize::Line&)> m_setupLine;
    std::function<void(geometrize::Polyline&)> m_setupPolyline;
    std::function<void(geometrize::QuadraticBezier&)> m_setupQuadraticBezier;
    std::function<void(geometrize::Rectangle&)> m_setupRectangle;
    std::function<void(geometrize::RotatedEllipse&)> m_setupRotatedEllipse;
    std::function<void(geometrize::RotatedRectangle&)> m_setupRotatedRectangle;
    std::function<void(geometrize::Triangle&)> m_setupTriangle;

    std::function<void(geometrize::Circle&)> m_mutateCircle;
    std::function<void(geometrize::Ellipse&)> m_mutateEllipse;
    std::function<void(geometrize::Line&)> m_mutateLine;
    std::function<void(geometrize::Polyline&)> m_mutatePolyline;
    std::function<void(geometrize::QuadraticBezier&)> m_mutateQuadraticBezier;
    std::function<void(geometrize::Rectangle&)> m_mutateRectangle;
    std::function<void(geometrize::RotatedEllipse&)> m_mutateRotatedEllipse;
    std::function<void(geometrize::RotatedRectangle&)> m_mutateRotatedRectangle;
    std::function<void(geometrize::Triangle&)> m_mutateTriangle;
};

}
