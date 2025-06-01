#include "object.h"

Object::Object(std::string path)
{
    auto result = loadFromFile(path);
    initFromVectors(result.first, result.second);
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
