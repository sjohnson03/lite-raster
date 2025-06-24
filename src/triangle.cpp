#include "triangle/triangle.h"

Triangle::Triangle(float2 a, float2 b, float2 c, Colour colour)
    : A(a), B(b), C(c)
{
    this->colour = colour;
    Az = 0.f;
    Bz = 0.f;
    Cz = 0.f;
}

Triangle::~Triangle()
{
}

bool Triangle::isPointInsideTriangle(const float2 &point) const
{
    bool inside = true;

    inside &= edgeFunction(A, B, point);
    inside &= edgeFunction(B, C, point);
    inside &= edgeFunction(C, A, point);

    return inside;
}

// Returns the bounding box for this triangle in form minX, maxX, minY, maxY
std::tuple<int, int, int, int> Triangle::getBoundingBox(int width, int height) const
{
    // Find min and max points
    float minXf = std::min(std::min(A.x, B.x), C.x);
    float maxXf = std::max(std::max(A.x, B.x), C.x);
    float minYf = std::min(std::min(A.y, B.y), C.y);
    float maxYf = std::max(std::max(A.y, B.y), C.y);

    // Clamp and convert to integers
    int minX = std::max(0, static_cast<int>(std::floor(minXf)));
    int maxX = std::min(width - 1, static_cast<int>(std::ceil(maxXf)));
    int minY = std::max(0, static_cast<int>(std::floor(minYf)));
    int maxY = std::min(height - 1, static_cast<int>(std::ceil(maxYf)));

    return {minX, maxX, minY, maxY};
}

float Triangle::getDepth() const
{
    // For any point P,  P = α * A + β * B + γ * C
    float z = alpha * Az + beta * Bz + gamma * Cz; // using the stored Z values, we can calculate the Z value for this point

    return -1.0f / z;
}

float3 Triangle::getWorldPosition() const
{
    float3 A3D = float3(A.x, A.y, Az);
    float3 B3D = float3(B.x, B.y, Bz);
    float3 C3D = float3(C.x, C.y, Cz);

    // For any point P, P = α*A + β*B + γ*C
    float3 worldPos = A3D * alpha + B3D * beta + C3D * gamma;

    return worldPos;
}
float3 Triangle::getNormal() const
{
    // Recreate vectors in 3D
    float3 A3D = float3(A.x, A.y, Az);
    float3 B3D = float3(B.x, B.y, Bz);
    float3 C3D = float3(C.x, C.y, Cz);

    // Calculate two edge vectors
    float3 edge1 = B3D - A3D;
    float3 edge2 = C3D - A3D;

    float3 normal = edge2.cross(edge1);
    return normal.normalise();
}

void Triangle::setColour(Colour colour)
{
    this->colour = colour;
}

Colour Triangle::getColour() const
{
    return colour;
}

// function from https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html
bool Triangle::edgeFunction(const float2 &a, const float2 &b, const float2 &c) const
{
    return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0);
}

void Triangle::calculateBarycentricCoordinates(const float2 &point)
{
    float2 v0 = B - A;
    float2 v1 = C - A;
    float2 v2 = point - A;

    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);

    denom = d00 * d11 - d01 * d01;
    beta = (d11 * d20 - d01 * d21) / denom;
    gamma = (d00 * d21 - d01 * d20) / denom;
    alpha = 1.0f - beta - gamma;
}
