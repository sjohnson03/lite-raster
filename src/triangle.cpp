#include "triangle/triangle.h"

Triangle::Triangle(float2 a, float2 b, float2 c, Colour colour)
    : A(a), B(b), C(c),
      AB(b - a),
      AC(a - c),
      BC(c - b)
{
    this->colour = colour;
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
