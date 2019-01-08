#pragma once

#include <cstdint>

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
class Shape;
class Triangle;
}

namespace geometrize
{

// Default implementations that perform initial setup on each type of shape
void setup(geometrize::Shape& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::Circle& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::Ellipse& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::Line& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::Polyline& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::QuadraticBezier& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::Rectangle& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::RotatedEllipse& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::RotatedRectangle& shape, std::int32_t xBound, std::int32_t yBound);
void setup(geometrize::Triangle& shape, std::int32_t xBound, std::int32_t yBound);

// Default implementations that mutate each type of shape
void mutate(geometrize::Shape& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::Circle& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::Ellipse& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::Line& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::Polyline& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::QuadraticBezier& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::Rectangle& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::RotatedEllipse& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::RotatedRectangle& shape, std::int32_t xBound, std::int32_t yBound);
void mutate(geometrize::Triangle& shape, std::int32_t xBound, std::int32_t yBound);

}
