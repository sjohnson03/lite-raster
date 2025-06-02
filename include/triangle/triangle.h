#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include <random>

class Triangle
{
public:
    Triangle(float2 a, float2 b, float2 c, Colour colour = Colour(255.0f, 255.0f, 255.0f));
    ~Triangle();

    float2 A, B, C;    // points of the triangle in 2D
    float2 AB, AC, BC; // vectors connecting the triangle

    bool isPointInsideTriangle(const float2 &point) const;
    std::tuple<int, int, int, int> getBoundingBox(int width, int height) const;

    void setColour(Colour colour);
    Colour getColour() const;

private:
    Colour colour;
    bool edgeFunction(const float2 &a, const float2 &b, const float2 &c) const;
};

#endif