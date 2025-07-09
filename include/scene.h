#ifndef SCENE_H
#define SCENE_H

#include "light.h"
#include "object.h"
#include "raylib.h"
#include "texture.h"
#include "triangle/triangle.h"
#include "types.h"
#include <map>
#include <utility>
#include <vector>

class Scene {
public:
  Scene();
  ~Scene();

  std::vector<std::pair<Object *, float3>>
      objects; // All objects currently in the scene and their pos
  std::vector<Light *> lights; // All lights in the scene
  std::vector<std::pair<Triangle *, float3>>
      triangles; // All triangles currently in the scene and their position

  void render(int width, int height, Color *buffer);

  Colour computeLighting(const float3 &point, const float3 &normal,
                         const float3 &viewPos, Light &Light);

  void add(Object &object, float3 position = float3(0, 0, 0));
  void add(Light &light);
  void addTri(Triangle &triangle,
              float3 position = float3(0, 0, 0)); // add triangle to the scene

  std::map<std::string, Object *> addedObjects;
  std::map<std::string, Light *> addedLights;

private:
  float3 cameraPosition;

  // rasterisers a given triangle onto the screen
  void rasterise(int width, int height, Color *buffer,
                 std::vector<float> *zBuffer, Triangle3D *triangle3D,
                 float3 objectPosition, LiteRaster::Texture *texture);
};

#endif