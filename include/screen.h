#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"

class Screen
{
public:
    // Converts from NDC (-1 to 1) to screen space (0 to width/height)
    static float2 toScreenSpace(const float3 &vertex, int width, int height);
    static float2 project(int width, int height, const float3 &point, float focalLength = 1.0f);

private:
    float focalLength = 1.0f;
};

#endif