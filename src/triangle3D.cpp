#include "triangle/triangle3D.h"

Triangle3D::Triangle3D(float3 a, float3 b, float3 c)
    : A(a), B(b), C(c)
{
    // Generate random default colour
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    colour = Colour(dist(gen) * 255, dist(gen) * 255, dist(gen) * 255);
}

Triangle3D::~Triangle3D()
{
}

Triangle Triangle3D::projectTo2D(int width, int height)
{
    // Perspective projection onto the XY plane
    float2 a = Screen::project(width, height, A, 1.0f);
    float2 b = Screen::project(width, height, B, 1.0f);
    float2 c = Screen::project(width, height, C, 1.0f);

    Triangle tri = Triangle(a, b, c, colour);
    // store depth of points in terms of the perspective projection
    tri.Az = 1.0f / A.z;
    tri.Bz = 1.0f / B.z;
    tri.Cz = 1.0f / C.z;

    return tri;
}
