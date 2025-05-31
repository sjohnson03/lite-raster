#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"

class Triangle
{
public:
    Triangle(float2 a, float2 b, float2 c);

    bool isPointInsideTriangle(const float2 &point);

private:
    float2 A, B, C;    // points fo the triangle
    float2 AB, AC, BC; // vectors connecting the triangle
    bool edgeFunction(const float2 &a, const float2 &b, const float2 &c);
};

#endif