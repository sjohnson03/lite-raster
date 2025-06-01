#ifndef SCREEN_H
#define SCREEN_H

#include "types.h"

// Converts from NDC (-1 to 1) to screen space (0 to width/height)
float2 toScreenSpace(const float3 &vertex, int width, int height);

#endif