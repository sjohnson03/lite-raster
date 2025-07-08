#include "scene.h"
#include <utility>

Scene::Scene() { cameraPosition = float3(0.0f, 0.0f, 0.0f); }

Scene::~Scene() {}

// Renders and generates a buffer for a single frame
void Scene::render(int width, int height, Color *buffer) {
  std::vector<float> zBuffer(
      width * height, INFINITY); // stores the depth of all pixels in the scene

  // Clear the buffer
  for (int i = 0; i < width * height; i++) {
    buffer[i] = BLACK;
    zBuffer[i] = INFINITY;
  }

  for (unsigned long i = 0; i < objects.size();
       i++) // project all triangles onto 2D
  {
    Object *object = objects[i].first;
    if (object) {
      // kinda scuffed way to do this, not projecting any objects that are
      // "behind the camera" when the camera can rotate, this will break
      if (object->getTransform().z > cameraPosition.z)
        continue;

      for (unsigned long j = 0; j < object->triangles.size(); j++) {
        rasterise(width, height, buffer, &zBuffer, object->triangles[j],
                  object->getTransform());
      }
    }
  }
}

void Scene::rasterise(int width, int height, Color *buffer,
                      std::vector<float> *zBuffer, Triangle3D *triangle3D,
                      float3 objectPosition) {

  // Convert from 3D to 2D
  Triangle triangle = triangle3D->projectTo2D(width, height);

  float3 vertexColours[3];

  float3 worldPosA = triangle3D->A.position + objectPosition;
  float3 worldPosB = triangle3D->B.position + objectPosition;
  float3 worldPosC = triangle3D->C.position + objectPosition;

  float3 viewDirection = (cameraPosition - worldPosA).normalise();
  float3 faceNormal = (worldPosB - worldPosA).cross(worldPosC - worldPosA);

  if (faceNormal.dot(viewDirection) <=
      0.0f) { // triangle is facing away, no need to draw it
    return;
  }

  Colour c0(0, 0, 0), c1(0, 0, 0), c2(0, 0, 0);

  for (Light *light : lights) {
    c0 += computeLighting(worldPosA, triangle3D->A.normal, cameraPosition,
                          *light);
    c1 += computeLighting(worldPosB, triangle3D->B.normal, cameraPosition,
                          *light);
    c2 += computeLighting(worldPosC, triangle3D->C.normal, cameraPosition,
                          *light);
  }

  vertexColours[0] = float3(c0.r / 255.0f, c0.g / 255.0f, c0.b / 255.0f);
  vertexColours[1] = float3(c1.r / 255.0f, c1.g / 255.0f, c1.b / 255.0f);
  vertexColours[2] = float3(c2.r / 255.0f, c2.g / 255.0f, c2.b / 255.0f);

  // draw where the triangle is
  auto [minX, maxX, minY, maxY] = triangle.getBoundingBox(width, height);

  float3 triColNorm(triangle.getColour().r / 255.0f,
                    triangle.getColour().g / 255.0f,
                    triangle.getColour().b / 255.0f);

  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      float2 point = float2(x, y);
      if (triangle.isPointInsideTriangle(point)) {
        triangle.calculateBarycentricCoordinates(point);

        float depth = triangle.getDepth();
        if (depth >
            zBuffer->at(y * width +
                        x)) // there is a closer pixel, do not draw over it
          continue;

        (*zBuffer)[y * width + x] = depth;

        // Convert to perspective-correct weights
        float z0 = triangle3D->A.position.z + objectPosition.z;
        float z1 = triangle3D->B.position.z + objectPosition.z;
        float z2 = triangle3D->C.position.z + objectPosition.z;

        float w0 = triangle.alpha / z0;
        float w1 = triangle.beta / z1;
        float w2 = triangle.gamma / z2;

        float wSum = w0 + w1 + w2;
        w0 /= wSum;
        w1 /= wSum;
        w2 /= wSum;

        float3 interpolatedLight =
            vertexColours[0] * w0 + // Vertex A lighting * alpha
            vertexColours[1] * w1 + // Vertex B lighting * beta
            vertexColours[2] * w2;  // Vertex C lighting * gamma

        float3 triColNorm(triangle.getColour().r / 255.0f,
                          triangle.getColour().g / 255.0f,
                          triangle.getColour().b / 255.0f);

        float3 c = triColNorm * interpolatedLight;

        // 2D triangle colours are stored as values from 0 - 1. Convert this to
        // be 0 - 255
        buffer[y * width + x] = Color{(unsigned char)(c.x * 255), // red
                                      (unsigned char)(c.y * 255), // green
                                      (unsigned char)(c.z * 255), // blue
                                      255};                       // alpha
      }
    }
  }
}

Colour Scene::computeLighting(const float3 &point, const float3 &normal,
                              const float3 &viewPos, Light &light) {
  float3 lightColour = float3(light.colour.r / 255.0f, light.colour.g / 255.0f,
                              light.colour.b / 255.0f);

  float3 lightDirection = (light.position - point);
  float lightDistance = lightDirection.length();
  float3 viewDirection = (viewPos - point).normalise();

  if (lightDistance < 0.001f) {
    // Light is at the same position as the point
    return Colour(255, 255, 255); // Full bright
  }

  lightDirection = lightDirection / lightDistance;

  // Ambient
  float3 ambient = lightColour * light.ambient;

  // Diffuse
  float diffuseIntensity = std::max(normal.dot(lightDirection), 0.0f);
  float3 diffuseColour = lightColour * diffuseIntensity * light.intensity;

  // Specular
  float specStrength = 0.2f;
  float shininess = 8.0f;

  float3 halfVector = (lightDirection + viewDirection).normalise();
  float specularIntensity =
      std::pow(std::max(normal.dot(halfVector), 0.0f), shininess);

  if (diffuseIntensity <= 0.0f) {
    specularIntensity = 0.0f;
  }

  float3 specular = lightColour * specStrength * specularIntensity;

  float3 finalColour = diffuseColour + ambient + specular;

  auto clamp = [](float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
  };
  finalColour.x = clamp(finalColour.x, 0.0f, 1.0f);
  finalColour.y = clamp(finalColour.y, 0.0f, 1.0f);
  finalColour.z = clamp(finalColour.z, 0.0f, 1.0f);

  return Colour(finalColour.x * 255, finalColour.y * 255, finalColour.z * 255);
}

void Scene::add(Object &object, float3 position) {
  objects.push_back(std::make_pair(&object, position));
  addedObjects[object.name] = &object;
}

void Scene::add(Light &light) {
  lights.push_back(&light);
  addedLights[light.name] = &light;
}

void Scene::addTri(Triangle &triangle, float3 position) {
  triangles.push_back(std::make_pair(&triangle, position));
}