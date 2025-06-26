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
    std::vector<float> zBuffer(width * height, INFINITY); // stores the depth of all pixels in the scene

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
                rasterise(width, height, buffer, &zBuffer, object->triangles[j], object->getTransform());
            }
        }
    }
}

void Scene::rasterise(int width, int height, Color *buffer, std::vector<float> *zBuffer, Triangle3D *triangle3D, float3 objectPosition)
{
    Light light;
    light.position = float3(0.f, 2.f, -5.0f);
    light.colour = Colour(255, 255, 255); // colour: white light
    light.intensity = 1.5f;               // intensity

    float3 vertexColours[3];

    float3 worldPosA = triangle3D->A.position + objectPosition;
    float3 worldPosB = triangle3D->B.position + objectPosition;
    float3 worldPosC = triangle3D->C.position + objectPosition;

    Colour c0 = computeLighting(worldPosA, triangle3D->A.normal, cameraPosition, light);
    Colour c1 = computeLighting(worldPosB, triangle3D->B.normal, cameraPosition, light);
    Colour c2 = computeLighting(worldPosC, triangle3D->C.normal, cameraPosition, light);

    vertexColours[0] = float3(c0.r / 255.0f, c0.g / 255.0f, c0.b / 255.0f);
    vertexColours[1] = float3(c1.r / 255.0f, c1.g / 255.0f, c1.b / 255.0f);
    vertexColours[2] = float3(c2.r / 255.0f, c2.g / 255.0f, c2.b / 255.0f);

    float z0 = triangle3D->A.position.z;
    float z1 = triangle3D->B.position.z;
    float z2 = triangle3D->C.position.z;

    // Rasterise
    Triangle triangle = triangle3D->projectTo2D(width, height);

    // draw where the triangle is
    auto [minX, maxX, minY, maxY] = triangle.getBoundingBox(width, height);

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            float2 point = float2(x, y);
            if (triangle.isPointInsideTriangle(point))
            {
                float depth = triangle.getDepth();
                if (depth > zBuffer->at(y * width + x)) // there is a closer pixel, do not draw over it
                    continue;

                (*zBuffer)[y * width + x] = depth;

                triangle.calculateBarycentricCoordinates(point);

                // weight barycentric coordinates by inverse depth
                float w0 = triangle.alpha / -z0; // alpha corresponds to vertex A
                float w1 = triangle.beta / -z1;  // beta corresponds to vertex B
                float w2 = triangle.gamma / -z2; // gamma corresponds to vertex C

                float wSum = w0 + w1 + w2;

                if (wSum > 0.0001f)
                {
                    w0 /= wSum;
                    w1 /= wSum;
                    w2 /= wSum;

                    float3 interpolatedLight =
                        vertexColours[0] * w0 + // Vertex A lighting * alpha
                        vertexColours[1] * w1 + // Vertex B lighting * beta
                        vertexColours[2] * w2;  // Vertex C lighting * gamma

                    float3 triColNorm(
                        triangle.getColour().r / 255.0f,
                        triangle.getColour().g / 255.0f,
                        triangle.getColour().b / 255.0f);

                    float3 c = triColNorm * interpolatedLight;

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
    float3 reflectionDir = lightDirection - normal * 2.0f * normal.dot(lightDirection);

    // Ambient
    float ambientStrength = 0.2f;
    float3 ambient = lightColour * ambientStrength;

    // Diffuse
    float diffuse = std::max(normal.dot(lightDirection), 0.0f);
    float3 diffuseColour = lightColour * diffuse * light.intensity;

    // Specular
    float specStrength = 0.5f;
    float shininess = 32.0f;
    float dotProduct = std::max(viewDirection.dot(reflectionDir), 0.0f);
    float spec = (dotProduct > 0.1f) ? std::pow(dotProduct, shininess) : 0.0f;
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