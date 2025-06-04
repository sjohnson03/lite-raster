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

    float2 operator/(float value) const
    {
        return float2(x / value, y / value);
    };

    float2
    perpendicular() const
    {
        return float2(-y, x);
    };
};

struct float3
{
    float x, y, z;
    float3() : x(0), y(0), z(0) {} // default constructor
    float3(float x, float y, float z) : x(x), y(y), z(z) {}

    float3 operator+(const float3 &other) const
    {
        return float3(x + other.x, y + other.y, z + other.z);
    };

    float3 operator*(const float3 &other) const
    {
        return float3(x * other.x, y * other.y, z * other.z);
    }

    float3 operator*(const float value) const
    {
        return float3(x * value, y * value, z * value);
    }
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
