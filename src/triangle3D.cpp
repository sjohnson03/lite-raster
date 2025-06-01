#include "triangle/triangle3D.h"

Triangle3D::Triangle3D(float3 a, float3 b, float3 c)
    : A(a), B(b), C(c)
{
    // Generate random default colour
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    colour = Colour(dist(gen), dist(gen), dist(gen));
}

Triangle3D::~Triangle3D()
{
}

Triangle Triangle3D::projectTo2D(int width, int height)
{
    // Simple orthographic projection onto the XY plane
    float2 a = toScreenSpace(A, width, height);
    float2 b = toScreenSpace(B, width, height);
    float2 c = toScreenSpace(C, width, height);

    return Triangle(a, b, c, colour);
}