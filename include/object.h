#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"
#include "triangle/triangle.h"
#include "triangle/triangle3D.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Object
{
public:
    Object(std::string path); // create from file
    ~Object();

    void initFromVectors(const std::vector<float3> &points, const std::vector<float3> &faces);

    static std::pair<std::vector<float3>, std::vector<float3>> loadFromFile(std::string file); // vertices and faces

    std::vector<Triangle3D *> triangles;
};

#endif