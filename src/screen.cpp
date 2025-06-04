#include "screen.h"

float2 Screen::project(int width, int height, const float3 &point, float focalLength)
{
    // Avoid division by zero
    float z = (point.z == 0.0f) ? 1e-5f : -point.z; // if it is 0, make it a very small number instead
    float x_proj = (focalLength * point.x) / z;
    float y_proj = (focalLength * point.y) / z;

    // Map projected coordinates to screen space
    float aspect = (float)width / (float)height;
    float screen_x = (x_proj / aspect + 1.0f) * 0.5f * width;
    float screen_y = (1.0f - y_proj) * 0.5f * height;
    return float2(screen_x, screen_y);
};
