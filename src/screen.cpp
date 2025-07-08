#include "screen.h"

float2 Screen::project(int width, int height, const float3 &point,
                       float focalLength) {
  // Avoid division by zero
  float z = (point.z == 0.0f)
                ? 1e-5f
                : -point.z; // if it is 0, make it a very small number instead
  float x_proj = (focalLength * point.x) / z;
  float y_proj = (focalLength * point.y) / z;

  // Map projected coordinates to screen space
  float aspect = (float)width / (float)height;
  float screen_x = (x_proj / aspect + 1.0f) * 0.5f * width;
  float screen_y = (1.0f - y_proj) * 0.5f * height;
  return float2(screen_x, screen_y);
}
float3 Screen::toWorldSpace(int width, int height, const float2 &screenPoint) {
  float aspect = (float)width / (float)height;

  // Normalize screen coordinates to [-1, 1]
  float x_ndc = (2.0f * screenPoint.x) / width - 1.0f;
  float y_ndc = 1.0f - (2.0f * screenPoint.y) / height;

  // Undo aspect ratio scaling
  float x_proj = x_ndc * aspect;
  float y_proj = y_ndc;

  // Assume a default depth (e.g., z = -1.0f) and focal length = 1.0f
  float focalLength = 1.0f;
  float z = -1.0f;

  float x = (x_proj * z) / focalLength;
  float y = (y_proj * z) / focalLength;

  return float3(x, y, z);
};
