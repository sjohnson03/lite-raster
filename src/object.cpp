#include "object.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct VertexIndex
{
    int vIdx;
    int vtIdx;
    int vnIdx;
};

Object::Object(std::string path)
{
    auto result = loadFromFile(path);
    initFromVectors(result.first.first, result.first.second, result.second);
    name = path;

    transform = float3(0.f, 0.f, 0.f);
    rotation = float3(0.f, 0.f, 0.f);
    scale = float3(1.0f, 1.0f, 1.0f);
}

void Object::initFromVectors(const std::vector<float3> &points, const std::vector<float3> &normals, const std::vector<float3> &faces)
{
    for (unsigned long i = 0; i < faces.size(); i++)
    {
        Vertex v0 = points.at(faces[i].x);
        Vertex v1 = points.at(faces[i].y);
        Vertex v2 = points.at(faces[i].z);

        v0.normal = normals.at(faces[i].x);
        v1.normal = normals.at(faces[i].y);
        v2.normal = normals.at(faces[i].z);

        Triangle3D *tri = new Triangle3D(v0, v1, v2);
        originalTriangles.push_back(tri);

        Triangle3D *cloned = new Triangle3D(v0, v1, v2);
        cloned->colour = tri->colour;
        triangles.push_back(cloned);
    }
}

Object::~Object()
{
    for (auto tri : triangles)
        delete tri;

    for (auto tri : originalTriangles)
        delete tri;
}

std::pair<std::pair<std::vector<float3>, std::vector<float3>>, std::vector<float3>> Object::loadFromFile(std::string path)
{
    std::pair<std::pair<std::vector<float3>, std::vector<float3>>, std::vector<float3>> result;

    std::ifstream file;
    file.open(path);
    if (file.is_open())
    {
        std::string line;

        while (std::getline(file, line))
        {
            if (line.rfind("vn ", 0) == 0) // if reading a vertex normal
            {
                std::stringstream ss(line);
                std::string prefix;
                float x, y, z;
                ss >> prefix >> x >> y >> z;
                result.first.second.push_back(float3{x, y, z});
            }
            else if (!line.empty() && line[0] == 'v') // if this is a vertex
            {
                std::stringstream ss(line);
                char type;
                float x, y, z;
                ss >> type >> x >> y >> z;
                result.first.first.push_back(float3{x, y, z});
            }
            else if (!line.empty() && line[0] == 'f') // if this a face
            {
                std::stringstream ss(line);
                char type;
                ss >> type;
                std::vector<VertexIndex> faceVerts;
                std::string vertex;
                while (ss >> vertex)
                {
                    std::stringstream vs(vertex);
                    std::string v, vt, vn;
                    std::getline(vs, v, '/');
                    std::getline(vs, vt, '/');
                    std::getline(vs, vn, '/');

                    VertexIndex vi;
                    vi.vIdx = std::stoi(v) - 1;
                    vi.vtIdx = vt.empty() ? -1 : std::stoi(vt) - 1;
                    vi.vnIdx = vn.empty() ? -1 : std::stoi(vn) - 1;
                    faceVerts.push_back(vi);
                }

                // Store as float3 (assuming faces are triangles)
                if (faceVerts.size() == 3)
                {
                    result.second.push_back(float3(
                        static_cast<float>(faceVerts[0].vIdx),
                        static_cast<float>(faceVerts[1].vIdx),
                        static_cast<float>(faceVerts[2].vIdx)));
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
    rotation = rotationDegrees * (M_PI / 180.0f);

    updateTransformedTriangles();
}

void Object::addRotation(float3 rotationDegrees)
{
    rotation = rotation + (rotationDegrees * (M_PI / 180.0f));

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

void Object::setColour(uint8_t r, uint8_t g, uint8_t b)
{
    Colour c = Colour(r, g, b);

    // set all triangles colour to this new colour
    for (Triangle3D *tri : triangles)
        tri->colour = c;

    for (Triangle3D *origTriangle : originalTriangles)
        origTriangle->colour = c;

    this->colour = c;
}

Colour Object::getColour()
{
    return colour;
}

void Object::updateTransformedTriangles()
{

    for (Triangle3D *tri : triangles)
        delete tri;

    triangles.clear();

    for (Triangle3D *tri : originalTriangles)
    {
        // Clone triangle vertices
        float3 A = tri->A.position;
        float3 B = tri->B.position;
        float3 C = tri->C.position;

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
