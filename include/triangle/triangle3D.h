#ifndef TRIANGLE3D_H
#define TRIANGLE3D_H

#include "screen.h"
#include "triangle/triangle.h"
#include "types.h"

struct Vertex {
  float3 position;
  float3 normal;
  float2 uv;

  Vertex(float3 pos, float3 norm, float2 uv)
      : position(pos), normal(norm), uv(uv) {};
  Vertex(float3 pos, float3 norm) : position(pos), normal(norm), uv({0, 0}) {}
  Vertex(float3 pos) : position(pos), normal({0, 0, 0}), uv({0, 0}) {}
};

class Triangle3D {
public:
  Triangle3D(float3 a, float3 b, float3 c);
  Triangle3D(Vertex a, Vertex b, Vertex c);
  ~Triangle3D();

  Triangle projectTo2D(int width, int height); // screen width and height

  Vertex A, B, C; // vertexes of the triangle in 3D
  Colour colour;  // colour the triangle represents
};

#endif