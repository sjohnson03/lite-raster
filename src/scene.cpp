#include "scene.h"

Scene::Scene()
{
    cameraPosition = float3(0.0f, 0.0f, 0.0f);
}

Scene::~Scene()
{
}

// Renders and generates a buffer for a single frame
void Scene::render(int width, int height, Color *buffer)
{
    std::vector<Triangle> triangles;
    float *zBuffer = new float[width * height]; // stores the depth of all pixels in the scene

    // Clear the buffer
    for (int i = 0; i < width * height; i++)
    {
        buffer[i] = BLACK;
        zBuffer[i] = INFINITY;
    }

    for (unsigned long i = 0; i < objects.size(); i++) // project all triangles onto 2D
    {
        Object *object = objects[i].first;
        if (object)
        {
            // kinda scuffed way to do this, not projecting any objects that are "behind the camera"
            // when the camera can rotate, this will break
            if (object->getTransform().z > cameraPosition.z)
                continue;

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
                    float depth = triangle.getDepth(point);
                    if (depth > zBuffer[y * width + x]) // there is a closer pixel, do not draw over it
                        continue;

                    zBuffer[y * width + x] = depth;

                    Colour c = triangle.getColour();

                    // 2D triangle colours are stored as values from 0 - 1. Convert this to be 0 - 255
                    buffer[y * width + x] = Color{
                        (unsigned char)(c.r), // red
                        (unsigned char)(c.g), // green
                        (unsigned char)(c.b), // blue
                        255};                 // alpha
                }
            }
        }
    }
    delete[] zBuffer;
}

void Scene::add(Object &object, float3 position)
{
    objects.push_back(std::make_pair(&object, position));
    addedObjects[object.name] = &object;
}

void Scene::addTri(Triangle &triangle, float3 position)
{
    triangles.push_back(std::make_pair(&triangle, position));
}