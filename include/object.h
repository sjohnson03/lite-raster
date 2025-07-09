#ifndef OBJECT_H
#define OBJECT_H

#include "texture.h"
#include "triangle/triangle3D.h"
#include "types.h"
#include <math.h>
#include <vector>

struct VertexIndex {
  int vIdx;
  int vtIdx;
  int vnIdx;
};

struct FaceInfo {
  VertexIndex faceInfo[3];
};

struct ShapeInformation {
  std::vector<float3> points;
  std::vector<float3> normals;
  std::vector<float2> textureCoordinates;
  std::vector<FaceInfo> faces;
};

class Object {
public:
  LiteRaster::Texture *texture;

  Object(std::string path); // create from file
  ~Object();

  void initFromVectors(ShapeInformation);

  static ShapeInformation loadFromFile(std::string file); // vertices and faces

  std::vector<Triangle3D *> triangles;
  std::string name = "Object";

  void setTransform(float3 position);
  float3 getTransform();

  void setRotation(float3 rotationDegrees);
  void addRotation(float3);
  float3 getRotation();

  void setScale(float3 scale);
  float3 getScale();

  void setColour(uint8_t r, uint8_t g, uint8_t b);
  Colour getColour();

  void updateTransformedTriangles();

private:
  float3 transform;
  float3 rotation;
  float3 scale;
  Colour colour;

  std::vector<Triangle3D *> originalTriangles;

  float3 rotateX(const float3 &v, float angle);
  float3 rotateY(const float3 &v, float angle);
  float3 rotateZ(const float3 &v, float angle);
};

#endif