#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"

class Matrix3x3 {
public:
  float matrix[3][3];

  Matrix3x3();
  Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12,
            float m20, float m21, float m22);

  float3 operator*(const float3 &other) const;
  Matrix3x3 operator*(const Matrix3x3 &other) const;

  Matrix3x3 transpose() const;
  Matrix3x3 inverse() const;
};

class Matrix4x4 {
public:
  float matrix[4][4];

  Matrix4x4();
  Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11,
            float m12, float m13, float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);

  float3 operator*(const float3 &other) const;
  Matrix4x4 operator*(const Matrix4x4 &other) const;

  Matrix3x3 getUpper3x3();
};

Matrix4x4 createScaleMatrix(float scale);
Matrix4x4 createScaleMatrix(float3 scale);
Matrix4x4 createTransformMatrix(float3 translation);
Matrix4x4 createRotationMatrixX(float angleRadians);
Matrix4x4 createRotationMatrixY(float angleRadians);
Matrix4x4 createRotationMatrixZ(float angleRadians);

Matrix4x4 createRotationMatrix(float xRad, float yRad, float zRad);
Matrix4x4 createRotationMatrix(float3 rotation);

#endif