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

                triangles.push_back(triangle);
            }
        }
    }

    rasterise(width, height, buffer, zBuffer, &triangles);

    delete[] zBuffer;
}

void Scene::rasterise(int width, int height, Color *buffer, float *zBuffer, std::vector<Triangle> *triangles)
{
    Light light;
    light.position = float3(2.0f, 2.0f, -2.0f); // position: above & behind the camera
    light.colour = Colour(255, 255, 255);       // colour: white light
    light.intensity = 1.0f;                     // intensity

    // Rasterise
    for (const Triangle &triangle : *triangles)
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

                    float3 center = triangle.center;
                    float3 normal = triangle.getNormal();

                    Colour lightCol = computeLighting(center, normal, cameraPosition, light);

                    float3 triColNorm(
                        triangle.getColour().r / 255.0f,
                        triangle.getColour().g / 255.0f,
                        triangle.getColour().b / 255.0f);

                    float3 lightColNorm(
                        lightCol.r / 255.0f,
                        lightCol.g / 255.0f,
                        lightCol.b / 255.0f);

                    float3 c = triColNorm * lightColNorm;

                    // 2D triangle colours are stored as values from 0 - 1. Convert this to be 0 - 255
                    buffer[y * width + x] = Color{
                        (unsigned char)(c.x * 255), // red
                        (unsigned char)(c.y * 255), // green
                        (unsigned char)(c.z * 255), // blue
                        255};                       // alpha
                }
            }
        }
    }
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
    float ambientStrength = 0.5f;
    float3 ambient = lightColour * ambientStrength * light.intensity;

    // Diffuse
    float diffuse = std::max(normal.dot(lightDirection), 0.0f);
    float3 diffuseColour = lightColour * diffuse * light.intensity;

    // Specular
    float specStrength = 0.5f;
    float shininess = 32.0f;
    float spec = std::pow(std::max(viewDirection.dot(reflectionDir), 0.0f), shininess);
    float3 specular = lightColour * specStrength * spec;

    float3 finalColour = ambient + diffuseColour + specular;

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