#include "scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::add(Object &object, float3 position)
{
    objects.push_back(std::make_pair(&object, position));
}

void Scene::addTri(Triangle &triangle, float3 position)
{
    triangles.push_back(std::make_pair(&triangle, position));
}