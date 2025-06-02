#include "scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

// Generates pixel buffer for the scene for the provided width and height
std::vector<std::vector<Pixel>> Scene::render(int width, int height)
{
    std::vector<Triangle> triangles;

    std::vector<std::vector<Pixel>> pixels(height, std::vector<Pixel>(width)); // clear the pixel buffer

    for (unsigned long i = 0; i < objects.size(); i++) // project all triangles onto 2D
    {
        Object *object = objects[i].first;
        if (object)
        {
            for (unsigned long j = 0; j < object->triangles.size(); j++)
            {
                Triangle triangle = object->triangles[j]->projectTo2D(width, height);

                triangles.push_back(triangle);
            }
        }
    }

    for (const Triangle &triangle : triangles)
    {
        // draw where there are triangles
        auto [minX, maxX, minY, maxY] = triangle.getBoundingBox(width, height);

        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                float2 point = float2(x, y);
                if (triangle.isPointInsideTriangle(point))
                {
                    pixels[y][x] = Pixel(triangle.getColour());
                }
            }
        }
    }

    return pixels;
}

void Scene::add(Object &object, float3 position)
{
    objects.push_back(std::make_pair(&object, position));
}

void Scene::addTri(Triangle &triangle, float3 position)
{
    triangles.push_back(std::make_pair(&triangle, position));
}