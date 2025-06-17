#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

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

    float3 operator/(const float value) const
    {
        return float3(x / value, y / value, z / value);
    }

    float3 normalise() const
    {
        float sum = x + y + z;
        return float3(x, y, z) / sum;
    }
};

struct Colour
{
    uint8_t r, g, b;
    Colour(uint8_t red = 1, uint8_t green = 1, uint8_t blue = 1)
        : r(red), g(green), b(blue) {}
};

struct Pixel
{
    Colour colour;
    Pixel(Colour colour = Colour(0, 0, 0)) : colour(colour) {};
};

#endif
