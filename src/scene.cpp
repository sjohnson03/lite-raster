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
                Vertex A = object->triangles[j]->A;
                Vertex B = object->triangles[j]->B;
                Vertex C = object->triangles[j]->C;
                Triangle triangle = object->triangles[j]->projectTo2D(width, height);

                triangle.center = (A.position + B.position + C.position) / 3.0f;
                triangle.normal = (A.normal + B.normal + C.normal).normalise();
                std::cout << (A.normal + B.normal + C.normal).z << std::endl;
                triangles.push_back(triangle);
            }
        }
    }

    Light light;
    light.position = float3(2.0f, 2.0f, -2.0f); // position: above & behind the camera
    light.colour = Colour(255, 255, 255);       // colour: white light
    light.intensity = 1.0f;                     // intensity
    // Rasterise
    for (const Triangle &triangle : triangles)
    {
        // draw where there are triangles
        auto [minX, maxX, minY, maxY] = triangle.getBoundingBox(width, height);

        minX = std::max(minX, 0);
        maxX = std::min(maxX, width - 1);
        minY = std::max(minY, 0);
        maxY = std::min(maxY, height - 1);
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
                    float3 center = triangle.center;
                    float3 normal = triangle.normal;

                    Colour lightCol = computeLighting(center, normal, cameraPosition, light) * triangle.getColour();
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

Colour Scene::computeLighting(const float3 &point, const float3 &normal, const float3 &viewPos, Light &light)
{
    float3 lightColour = float3(
        light.colour.r / 255.0f,
        light.colour.g / 255.0f,
        light.colour.b / 255.0f);

    float3 lightDirection = (light.position - point).normalise();
    float3 viewDirection = (viewPos - point).normalise();
    float3 reflectionDir = normal * normal.dot(lightDirection) * 2.0f - lightDirection;

    // Ambient
    float ambientStrength = 0.1f;
    float3 ambient = lightColour * ambientStrength;

    float3 finalColour = ambient; // diffuseColour;
    auto clamp = [](float v, float lo, float hi)
    { return v < lo ? lo : (v > hi ? hi : v); };
    finalColour.x = clamp(finalColour.x, 0.0f, 1.0f);
    finalColour.y = clamp(finalColour.y, 0.0f, 1.0f);
    finalColour.z = clamp(finalColour.z, 0.0f, 1.0f);

    return Colour(finalColour.x * 255, finalColour.y * 255, finalColour.z * 255);
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