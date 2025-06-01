#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include "types.h"
#include "screen.h"
#include "triangle/triangle.h"

class Triangle3D
{
public:
    Triangle3D(float3 a, float3 b, float3 c);
    ~Triangle3D();

    Triangle projectTo2D(int width, int height); // screen width and height

private:
    float3 A, B, C; // vertexes of the triangle in 3D
    Colour colour;  // colour the triangle represents
};

#endif