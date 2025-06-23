#ifndef LIGHT_H
#define LIGHT_H

#include "types.h"

struct Light
{
    float3 position;
    Colour colour;
    float intensity;

    Light()
        : position(float3(0.f, 0.f, 0.f)), colour(Colour(0, 0, 0)), intensity(0.f) {}
};

#endif