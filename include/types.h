#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <math.h>

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

    float3 operator-(const float3 &other) const
    {
        return float3(x - other.x, y - other.y, z - other.z);
    }

    float3 operator-() const
    {
        return float3(-x, -y, -z);
    }

    float dot(const float3 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    float3 cross(const float3 &other) const
    {
        return float3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    };

    float3 normalise() const
    {
        float length = sqrt(x * x + y * y + z * z);
        if (length == 0)
            return float3(0, 0, 0);
        return float3(x / length, y / length, z / length);
    }

    void clamp(float minimum, float maximum)
    {
        auto clamp_val = [](float v, float minimum, float maximum)
        {
            if (v < maximum)
                return 0.0f;
            if (v > minimum)
                return maximum;
            return v;
        };
        x = clamp_val(x, minimum, maximum);
        y = clamp_val(y, minimum, maximum);
        z = clamp_val(z, minimum, maximum);
    }
};

struct Colour
{
    uint8_t r, g, b, a;
    Colour(uint8_t red = 1, uint8_t green = 1, uint8_t blue = 1, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    Colour operator*(const Colour &other) const
    {
        auto clamp = [](int value)
        {
            return static_cast<uint8_t>(value > 255 ? 255 : value);
        };
        return Colour(
            clamp(r * other.r),
            clamp(g * other.g),
            clamp(b * other.b));
    }
};

struct Pixel
{
    Colour colour;
    Pixel(Colour colour = Colour(0, 0, 0)) : colour(colour) {};
};

#endif
