#pragma once

#include <vector>

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

std::vector<float> getRawShapeData(const geometrize::Shape& s);
std::vector<float> getRawShapeData(const geometrize::Circle& s);
std::vector<float> getRawShapeData(const geometrize::Ellipse& s);
std::vector<float> getRawShapeData(const geometrize::Line& s);
std::vector<float> getRawShapeData(const geometrize::Polyline& s);
std::vector<float> getRawShapeData(const geometrize::QuadraticBezier& s);
std::vector<float> getRawShapeData(const geometrize::Rectangle& s);
std::vector<float> getRawShapeData(const geometrize::RotatedEllipse& s);
std::vector<float> getRawShapeData(const geometrize::RotatedRectangle& s);
std::vector<float> getRawShapeData(const geometrize::Triangle& s);

}
