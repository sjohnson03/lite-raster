#include "object.h"

Object::Object(std::string path)
{
    auto result = loadFromFile(path);
    initFromVectors(result.first, result.second);

    transform = float3(0.f, 0.f, 0.f);
    rotation = float3(0.f, 0.f, 0.f);
    scale = float3(1.0f, 1.0f, 1.0f);
}

void Object::initFromVectors(const std::vector<float3> &points, const std::vector<float3> &faces)
{
    for (unsigned long i = 0; i < faces.size(); i++)
    {
        float3 v0 = points.at(faces[i].x);
        float3 v1 = points.at(faces[i].y);
        float3 v2 = points.at(faces[i].z);

        Triangle3D *tri = new Triangle3D(v0, v1, v2);
        triangles.push_back(tri);
    }

    originalTriangles = triangles;
}

Object::~Object()
{
}

std::pair<std::vector<float3>, std::vector<float3>> Object::loadFromFile(std::string path)
{
    std::pair<std::vector<float3>, std::vector<float3>> result;

    std::ifstream file;
    file.open(path);
    if (file.is_open())
    {
        std::string line;

        while (std::getline(file, line))
        {
            if (!line.empty() && line[0] == 'v') // if this is a vertex
            {
                std::stringstream ss(line);
                char type;
                float x, y, z;
                ss >> type >> x >> y >> z;
                result.first.push_back(float3{x, y, z});
            }
            else if (!line.empty() && line[0] == 'f') // if this a face
            {
                std::stringstream ss(line);
                char type;
                ss >> type;
                std::vector<float> indices;
                std::string vertex;
                while (ss >> vertex)
                {
                    std::stringstream vs(vertex);
                    std::string indexStr;
                    std::getline(vs, indexStr, '/');
                    int idx = std::stoi(indexStr) - 1; // 0-indexed
                    indices.push_back(static_cast<float>(idx));
                }
                // Store as float3 (assuming faces are triangles)
                if (indices.size() == 3)
                {
                    result.second.push_back(float3{indices[0], indices[1], indices[2]});
                }
            }
        }

        file.close();
    }
    return result;
}

void Object::setTransform(float3 position)
{
    transform = position;

    updateTransformedTriangles();
}

float3 Object::getTransform()
{
    return transform;
}

void Object::setRotation(float3 rotationDegrees)
{
    // convert rotation to radians
    rotation.x = rotationDegrees.x * M_PI / 180.0f;
    rotation.y = rotationDegrees.y * M_PI / 180.0f;
    rotation.z = rotationDegrees.z * M_PI / 180.0f;

    updateTransformedTriangles();
}

float3 Object::getRotation()
{
    return rotation;
}

void Object::setScale(float3 scale)
{
    this->scale = scale;

    updateTransformedTriangles();
}

float3 Object::getScale()
{
    return scale;
}

void Object::updateTransformedTriangles()
{
    triangles.clear();

    for (Triangle3D *tri : originalTriangles)
    {
        // Clone triangle vertices
        float3 A = tri->A;
        float3 B = tri->B;
        float3 C = tri->C;

        // Scale
        A = A * scale;
        B = B * scale;
        C = C * scale;

        // Rotate (around origin)
        A = rotateX(A, rotation.x);
        A = rotateY(A, rotation.y);
        A = rotateZ(A, rotation.z);

        B = rotateX(B, rotation.x);
        B = rotateY(B, rotation.y);
        B = rotateZ(B, rotation.z);

        C = rotateX(C, rotation.x);
        C = rotateY(C, rotation.y);
        C = rotateZ(C, rotation.z);

        // Translate
        A = A + transform;
        B = B + transform;
        C = C + transform;

        Triangle3D *newTri = new Triangle3D(A, B, C);
        newTri->colour = tri->colour;
        triangles.push_back(newTri);
    }
}

float3 Object::rotateX(const float3 &v, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);
    return float3(
        v.x,
        v.y * cosA - v.z * sinA,
        v.y * sinA + v.z * cosA);
}

float3 Object::rotateY(const float3 &v, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);
    return float3(
        v.x * cosA + v.z * sinA,
        v.y,
        -v.x * sinA + v.z * cosA);
}

float3 Object::rotateZ(const float3 &v, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);
    return float3(
        v.x * cosA - v.y * sinA,
        v.x * sinA + v.y * cosA,
        v.z);
}
