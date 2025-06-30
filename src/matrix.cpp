#include "matrix.h"
#include "types.h"
#include <cmath>

// ------------------------- 3x3 Matrix -------------------------

Matrix3x3::Matrix3x3() {
  // init to identity matrix
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      matrix[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

Matrix3x3::Matrix3x3(float m00, float m01, float m02, float m10, float m11,
                     float m12, float m20, float m21, float m22) {
  matrix[0][0] = m00;
  matrix[0][1] = m01;
  matrix[0][2] = m02;
  matrix[1][0] = m10;
  matrix[1][1] = m11;
  matrix[1][2] = m12;
  matrix[2][0] = m20;
  matrix[2][1] = m21;
  matrix[2][2] = m22;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &other) const {
  // For multiplying two matrices
  Matrix3x3 result;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      result.matrix[i][j] = 0.0f;
      for (int k = 0; k < 3; ++k) {
        result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
      }
    }
  }
  return result;
}

float3 Matrix3x3::operator*(const float3 &other) const {
  return float3(
      matrix[0][0] * other.x + matrix[0][1] * other.y + matrix[0][2] * other.z,
      matrix[1][0] * other.x + matrix[0][1] * other.y + matrix[0][2] * other.z,
      matrix[2][0] * other.x + matrix[2][1] * other.y + matrix[2][2] * other.z);
}

Matrix3x3 Matrix3x3::transpose() const {
  return Matrix3x3(matrix[0][0], matrix[1][0], matrix[2][0], matrix[0][1],
                   matrix[1][1], matrix[2][1], matrix[0][2], matrix[1][2],
                   matrix[2][2]);
}

Matrix3x3 Matrix3x3::inverse() const {
  float det = matrix[0][0] *
                  (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
              matrix[0][1] *
                  (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
              matrix[0][2] *
                  (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);

  if (std::abs(det) < 1e-6f) {
    // Return identity if cannot be reversed
    return Matrix3x3();
  }

  float invDet = 1.0f / det;

  return Matrix3x3(
      (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) * invDet,
      (matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]) * invDet,
      (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) * invDet,
      (matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]) * invDet,
      (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) * invDet,
      (matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2]) * invDet,
      (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]) * invDet,
      (matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1]) * invDet,
      (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) * invDet);
}

// ------------------------- 4x4 Matrix -------------------------

// Defines a 4x4 matrix
Matrix4x4::Matrix4x4() {
  // init to identity matrix
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matrix[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03, float m10,
                     float m11, float m12, float m13, float m20, float m21,
                     float m22, float m23, float m30, float m31, float m32,
                     float m33) {
  matrix[0][0] = m00;
  matrix[0][1] = m01;
  matrix[0][2] = m02;
  matrix[0][3] = m03;
  matrix[1][0] = m10;
  matrix[1][1] = m11;
  matrix[1][2] = m12;
  matrix[1][3] = m13;
  matrix[2][0] = m20;
  matrix[2][1] = m21;
  matrix[2][2] = m22;
  matrix[2][3] = m23;
  matrix[3][0] = m30;
  matrix[3][1] = m31;
  matrix[3][2] = m32;
  matrix[3][3] = m33;
}

float3 Matrix4x4::operator*(const float3 &other) const {
  float w = matrix[3][0] * other.x + matrix[3][1] * other.y +
            matrix[3][2] * other.z + matrix[3][3];

  return float3(matrix[0][0] * other.x + matrix[0][1] * other.y +
                    matrix[0][2] * other.z / w,
                matrix[1][0] * other.x + matrix[1][1] * other.y +
                    matrix[1][2] * other.z / w,
                matrix[2][0] * other.x + matrix[2][1] * other.y +
                    matrix[2][2] * other.z / w);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const {
  Matrix4x4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.matrix[i][j] = 0;
      for (int k = 0; k < 4; k++) {
        result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
      }
    }
  }
  return result;
}

Matrix3x3 Matrix4x4::getUpper3x3() {
  return Matrix3x3(matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0],
                   matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1],
                   matrix[2][2]);
}

// ------------------------- Helper functions -------------------------

// Creates a basic scale matrix based on the scale provided
Matrix4x4 createScaleMatrix(float scale) {
  return Matrix4x4(scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, scale, 0, 0, 0, 0, 1);
}

Matrix4x4 createScaleMatrix(float3 scale) {
  return Matrix4x4(scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0,
                   0, 1);
}

Matrix4x4 createTransformMatrix(float3 translation) {
  return Matrix4x4(1, 0, 0, translation.x, 0, 1, 0, translation.y, 0, 0, 1,
                   translation.z, 0, 0, 0, 1);
}

Matrix4x4 createRotationMatrixX(float angleRadians) {
  float c = cos(angleRadians);
  float s = sin(angleRadians);
  return Matrix4x4(1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1);
}

Matrix4x4 createRotationMatrixY(float angleRadians) {
  float c = cos(angleRadians);
  float s = sin(angleRadians);
  return Matrix4x4(c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1);
}

Matrix4x4 createRotationMatrixZ(float angleRadians) {
  float c = cos(angleRadians);
  float s = sin(angleRadians);
  return Matrix4x4(c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix4x4 createRotationMatrix(float xRad, float yRad, float zRad) {
  // Create individual rotation matrices
  Matrix4x4 rotX = createRotationMatrixX(xRad);
  Matrix4x4 rotY = createRotationMatrixY(yRad);
  Matrix4x4 rotZ = createRotationMatrixZ(zRad);

  // Combine rotations (order: Z * Y * X)
  return rotZ * rotY * rotX;
}

Matrix4x4 createRotationMatrix(const float3 &rotation) {
  return createRotationMatrix(rotation.x, rotation.y, rotation.z);
}