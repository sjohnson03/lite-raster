#include "object.h"
#include "matrix.h"
#include "types.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

Object::Object(std::string path) {
  auto result = loadFromFile(path);
  initFromVectors(result);
  name = path;

  transform = float3(0.f, 0.f, 0.f);
  rotation = float3(0.f, 0.f, 0.f);
  scale = float3(1.0f, 1.0f, 1.0f);
}

void Object::initFromVectors(ShapeInformation shapeInfo) {

  std::vector<float3> points = shapeInfo.points;
  std::vector<float3> normals = shapeInfo.normals;
  std::vector<FaceInfo> faces = shapeInfo.faces;

  for (unsigned long i = 0; i < faces.size(); i++) {
    Vertex v0 = points.at(faces[i].faceInfo[0].vIdx);
    Vertex v1 = points.at(faces[i].faceInfo[1].vIdx);
    Vertex v2 = points.at(faces[i].faceInfo[2].vIdx);

    v0.normal = normals.at(faces[i].faceInfo[0].vnIdx);
    v1.normal = normals.at(faces[i].faceInfo[1].vnIdx);
    v2.normal = normals.at(faces[i].faceInfo[2].vnIdx);

    Triangle3D *tri = new Triangle3D(v0, v1, v2);
    originalTriangles.push_back(tri);

    Triangle3D *cloned = new Triangle3D(v0, v1, v2);
    cloned->colour = tri->colour;
    triangles.push_back(cloned);
  }
}

Object::~Object() {
  for (auto tri : triangles)
    delete tri;

  for (auto tri : originalTriangles)
    delete tri;
}

ShapeInformation Object::loadFromFile(std::string path) {
  ShapeInformation result;

  std::ifstream file;
  file.open(path);
  if (file.is_open()) {
    std::string line;

    while (std::getline(file, line)) {
      if (line.rfind("vn ", 0) == 0) // if reading a vertex normal
      {
        std::stringstream ss(line);
        std::string prefix;
        float x, y, z;
        ss >> prefix >> x >> y >> z;
        result.normals.push_back(float3{x, y, z});
      } else if (!line.empty() && line[0] == 'v') // if this is a vertex
      {
        std::stringstream ss(line);
        char type;
        float x, y, z;
        ss >> type >> x >> y >> z;
        result.points.push_back(float3{x, y, z});
      } else if (!line.empty() && line[0] == 'f') // if this a face
      {
        std::stringstream ss(line);
        char type;
        ss >> type;
        std::vector<VertexIndex> faceVerts;
        std::string vertex;
        while (ss >> vertex) {
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

        // Store face information
        if (faceVerts.size() == 3) {
          FaceInfo faceInfo;
          faceInfo.faceInfo[0] = faceVerts[0];
          faceInfo.faceInfo[1] = faceVerts[1];
          faceInfo.faceInfo[2] = faceVerts[2];

          result.faces.push_back(faceInfo);
        }
      }
    }

    file.close();
  }
  return result;
}

void Object::setTransform(float3 position) {
  transform = position;

  updateTransformedTriangles();
}

float3 Object::getTransform() { return transform; }

void Object::setRotation(float3 rotationDegrees) {
  // convert rotation to radians
  rotation = rotationDegrees * (M_PI / 180.0f);

  updateTransformedTriangles();
}

void Object::addRotation(float3 rotationDegrees) {
  rotation = rotation + (rotationDegrees * (M_PI / 180.0f));

  updateTransformedTriangles();
}

float3 Object::getRotation() { return rotation; }

void Object::setScale(float3 scale) {
  this->scale = scale;

  updateTransformedTriangles();
}

float3 Object::getScale() { return scale; }

void Object::setColour(uint8_t r, uint8_t g, uint8_t b) {
  Colour c = Colour(r, g, b);

  // set all triangles colour to this new colour
  for (Triangle3D *tri : triangles)
    tri->colour = c;

  for (Triangle3D *origTriangle : originalTriangles)
    origTriangle->colour = c;

  this->colour = c;
}

Colour Object::getColour() { return colour; }

void Object::updateTransformedTriangles() {
  // Clear existing triangles
  for (Triangle3D *tri : triangles)
    delete tri;
  triangles.clear();

  // Create transformation matrix once
  Matrix4x4 rotationMatrix =
      createRotationMatrix(rotation.x, rotation.y, rotation.z);
  Matrix4x4 scaleMatrix = createScaleMatrix(scale);
  Matrix4x4 transformMatrix = rotationMatrix * scaleMatrix;

  // For normals, use the inverse transpose of the upper 3x3 rotation matrix
  Matrix3x3 normalMatrix = rotationMatrix.getUpper3x3().inverse().transpose();

  for (Triangle3D *tri : originalTriangles) {
    // Transform vertices
    Vertex A = tri->A;
    Vertex B = tri->B;
    Vertex C = tri->C;

    // Apply scale and rotation
    A.position = transformMatrix * A.position;
    B.position = transformMatrix * B.position;
    C.position = transformMatrix * C.position;

    // Transform normals (rotation only, no scale)
    A.normal = (normalMatrix * A.normal).normalise();
    B.normal = (normalMatrix * B.normal).normalise();
    C.normal = (normalMatrix * C.normal).normalise();

    // Apply translation
    A.position = A.position + transform;
    B.position = B.position + transform;
    C.position = C.position + transform;

    Triangle3D *newTri = new Triangle3D(A, B, C);
    newTri->colour = tri->colour;
    triangles.push_back(newTri);
  }
}

float3 Object::rotateX(const float3 &v, float angle) {
  float cosA = cos(angle);
  float sinA = sin(angle);
  return float3(v.x, v.y * cosA - v.z * sinA, v.y * sinA + v.z * cosA);
}

float3 Object::rotateY(const float3 &v, float angle) {
  float cosA = cos(angle);
  float sinA = sin(angle);
  return float3(v.x * cosA + v.z * sinA, v.y, -v.x * sinA + v.z * cosA);
}

float3 Object::rotateZ(const float3 &v, float angle) {
  float cosA = cos(angle);
  float sinA = sin(angle);
  return float3(v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA, v.z);
}
