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

    void setTransform(float3 position);
    float3 getTransform();

    void setRotation(float3 rotationDegrees);
    void addRotation(float3);
    float3 getRotation();

    void setScale(float3 scale);
    float3 getScale();

    void updateTransformedTriangles();

private:
    float3 transform;
    float3 rotation;
    float3 scale;

    std::vector<Triangle3D *> originalTriangles;

    float3 rotateX(const float3 &v, float angle);
    float3 rotateY(const float3 &v, float angle);
    float3 rotateZ(const float3 &v, float angle);
};

#endif