#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include "types.h"
#include "screen.h"
#include "triangle/triangle.h"

struct Vertex
{
    float3 position;
    float3 normal;

    Vertex(float3 pos, float3 norm) : position(pos), normal(norm) {}
    Vertex(float3 pos) : position(pos) {}
};

class Triangle3D
{
public:
    Triangle3D(float3 a, float3 b, float3 c);
    Triangle3D(Vertex a, Vertex b, Vertex c);
    ~Triangle3D();

    Triangle projectTo2D(int width, int height); // screen width and height

    Vertex A, B, C; // vertexes of the triangle in 3D
    Colour colour;  // colour the triangle represents
};

#endif