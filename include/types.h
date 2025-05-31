#ifndef TYPES_H
#define TYPES_H

struct float2
{
    float x, y;
    float2(float x, float y) : x(x), y(y) {}

    float2 operator-(const float2 &other) const
    {
        return float2(x - other.x, y - other.y);
    }

    float2 operator+(const float2 &other) const
    {
        return float2(x + other.x, y + other.y);
    };

    float dot(const float2 &other) const
    {
        return x * other.x + y * other.y;
    };

    float2 perpendicular() const
    {
        return float2(-y, x);
    };
};

struct float3
{
    float x, y, z;
    float3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Colour
{
    float r, g, b;
    Colour(float red = 0.0f, float green = 0.0f, float blue = 0.0f)
        : r(red), g(green), b(blue) {}
};

struct Pixel
{
    Colour colour;
    Pixel(Colour colour = Colour(0.0f, 0.0f, 0.0f)) : colour(colour) {};
};

#endif
