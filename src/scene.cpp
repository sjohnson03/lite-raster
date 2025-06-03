#include "scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

// Renders and generates a buffer for a single frame
void Scene::render(int width, int height, Color *buffer)
{
    std::vector<Triangle> triangles;

    // Clear the buffer
    for (int i = 0; i < width * height; i++)
        buffer[i] = BLACK;

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
                    Colour c = triangle.getColour();

                    // 2D triangle colours are stored as values from 0 - 1. Convert this to be 0 - 255
                    buffer[y * width + x] = Color{
                        (unsigned char)(c.r * 255), // red
                        (unsigned char)(c.g * 255), // green
                        (unsigned char)(c.b * 255), // blue
                        255};                       // alpha
                }
            }
        }
    }
}

void Scene::add(Object &object, float3 position)
{
    objects.push_back(std::make_pair(&object, position));
}

void Scene::addTri(Triangle &triangle, float3 position)
{
    triangles.push_back(std::make_pair(&triangle, position));
}