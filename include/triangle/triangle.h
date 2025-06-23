#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include <random>

class Triangle
{
public:
    Triangle(float2 a, float2 b, float2 c, Colour colour = Colour(255, 255, 255));
    ~Triangle();

    float2 A, B, C;   // points of the triangle in 2D
    float Az, Bz, Cz; // depth of each point

    // Saved information from conversion from 3D
    float3 center;
    float3 normal; // average normal for the whole triangle

    bool isPointInsideTriangle(const float2 &point) const;
    std::tuple<int, int, int, int> getBoundingBox(int width, int height) const;

    float getDepth(float2 point) const;

    void setColour(Colour colour);
    Colour getColour() const;

private:
    Colour colour;
    bool edgeFunction(const float2 &a, const float2 &b, const float2 &c) const;
};

#endif