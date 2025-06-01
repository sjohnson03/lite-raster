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

bool Triangle::isPointInsideTriangle(const float2 &point)
{
    bool inside = true;

    inside &= edgeFunction(A, B, point);
    inside &= edgeFunction(B, C, point);
    inside &= edgeFunction(C, A, point);

    return inside;
}

void Triangle::setColour(Colour colour)
{
    this->colour = colour;
}

Colour Triangle::getColour()
{
    return colour;
}

// function from https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html
bool Triangle::edgeFunction(const float2 &a, const float2 &b, const float2 &c)
{
    return ((c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x) >= 0);
}
