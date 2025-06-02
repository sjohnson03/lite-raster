#include "screen.h"

// Converts from NDC (-1 to 1) to screen space (0 to width/height)
float2 toScreenSpace(const float3 &vertex, int width, int height)
{
    // Calculate aspect ratio
    float aspect = (float)width / (float)height;

    float screenX = (vertex.x / aspect + 1.0f) * 0.5f * width; // Apply aspect ratio
    float screenY = (1.0f - vertex.y) * 0.5f * height;         // Flip Y to match screen coords
    return float2(screenX, screenY);
}
