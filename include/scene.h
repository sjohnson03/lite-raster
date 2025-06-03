#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "triangle/triangle.h"
#include "raylib.h"
#include <vector>

class Scene
{
public:
    Scene();
    ~Scene();

    std::vector<std::pair<Object *, float3>> objects;     // All objects currently in the scene and their pos
    std::vector<std::pair<Triangle *, float3>> triangles; // All triangles currently in the scene and their position

    void render(int width, int height, Color *buffer);

    void add(Object &object, float3 position = float3(0, 0, 0));
    void addTri(Triangle &triangle, float3 position = float3(0, 0, 0)); // add triangle to the scene
};

#endif