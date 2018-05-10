#include "Matrix4x4.h"

namespace g3dcommon {

  Matrix4x4 Matrix4x4::Identity()
  {
    Matrix4x4 A;
    A(0, 0) = 1.f; A(0, 1) = 0.f; A(0, 2) = 0.f; A(0, 3) = 0.f;
    A(1, 0) = 0.f; A(1, 1) = 1.f; A(1, 2) = 0.f; A(1, 3) = 0.f;
    A(2, 0) = 0.f; A(2, 1) = 0.f; A(2, 2) = 1.f; A(2, 3) = 0.f;
    A(3, 0) = 0.f; A(3, 1) = 0.f; A(3, 2) = 0.f; A(3, 3) = 1.f;

    return A;
  }

  Matrix4x4 Matrix4x4::Rotation(float theta, Axis axis)
  {
    Matrix4x4 A;
    switch (axis)
    {
    case Axis::X:
      A(0, 0) = 1.f;        A(0, 1) = 0.f;        A(0, 2) = 0.f;        A(0, 3) = 0.f;
      A(1, 0) = 0.f;        A(1, 1) = cos(theta); A(1, 2) = sin(theta); A(1, 3) = 0.f;
      A(2, 0) = 0.f;        A(2, 1) =-sin(theta); A(2, 2) = cos(theta); A(2, 3) = 0.f;
      A(3, 0) = 0.f;        A(3, 1) = 0.f;        A(3, 2) = 0.f;        A(3, 3) = 1.f;
      break;
    case Axis::Y:
      A(0, 0) = cos(theta); A(0, 1) = 0.f;        A(0, 2) =-sin(theta); A(0, 3) = 0.f;
      A(1, 0) = 0.f;        A(1, 1) = 1.f;        A(1, 2) = 0.f;        A(1, 3) = 0.f;
      A(2, 0) = sin(theta); A(2, 1) = 0.f;        A(2, 2) = cos(theta); A(2, 3) = 0.f;
      A(3, 0) = 0.f;        A(3, 1) = 0.f;        A(3, 2) = 0.f;        A(3, 3) = 1.f;
      break;
    case Axis::Z:
      A(0, 0) = cos(theta); A(0, 1) = sin(theta); A(0, 2) = 0.f;        A(0, 3) = 0.f;
      A(1, 0) =-sin(theta); A(1, 1) = cos(theta); A(1, 2) = 0.f;        A(1, 3) = 0.f;
      A(2, 0) = 0.f;        A(2, 1) = 0.f;        A(2, 2) = 1.f;        A(2, 3) = 0.f;
      A(3, 0) = 0.f;        A(3, 1) = 0.f;        A(3, 2) = 0.f;        A(3, 3) = 1.f;
      break;
    default:
      break;
    }

    return A;
  }

  Matrix4x4 Matrix4x4::Translation(Vector3D t)
  {
    Matrix4x4 A = Matrix4x4::Identity();
    A(3, 0) = t.x; 
    A(3, 1) = t.y;
    A(3, 2) = t.z;

    return A;
  }

  Matrix4x4 Matrix4x4::Scaling(Vector3D s)
  {
    Matrix4x4 A = Matrix4x4::Identity();
    A(0, 0) = s.x;
    A(1, 1) = s.y;
    A(2, 2) = s.z;

    return A;
  }

  float Matrix4x4::Det() const
  {
  return
    m03*m12*m21*m30 - m02*m13*m21*m30 -
    m03*m11*m22*m30 + m01*m13*m22*m30 +
    m02*m11*m23*m30 - m01*m12*m23*m30 -
    m03*m12*m20*m31 + m02*m13*m20*m31 +
    m03*m10*m22*m31 - m00*m13*m22*m31 -
    m02*m10*m23*m31 + m00*m12*m23*m31 +
    m03*m11*m20*m32 - m01*m13*m20*m32 -
    m03*m10*m21*m32 + m00*m13*m21*m32 +
    m01*m10*m23*m32 - m00*m11*m23*m32 -
    m02*m11*m20*m33 + m01*m12*m20*m33 +
    m02*m10*m21*m33 - m00*m12*m21*m33 -
    m01*m10*m22*m33 + m00*m11*m22*m33;
  }

  Matrix4x4 Matrix4x4::T() const
  {
    Matrix4x4 A(m00, m10, m20, m30,
                m01, m11, m21, m31,
                m02, m12, m22, m32,
                m03, m13, m23, m33);

    return A;
  }

  Matrix4x4 Matrix4x4::Inv() const
  {
    // A is standard adjoint matrix.
    Matrix4x4 A;
    A.m00 = m12*m23*m31 - m13*m22*m31 + m13*m21*m32 - m11*m23*m32 - m12*m21*m33 + m11*m22*m33;
    A.m01 = m03*m22*m31 - m02*m23*m31 - m03*m21*m32 + m01*m23*m32 + m02*m21*m33 - m01*m22*m33;
    A.m02 = m02*m13*m31 - m03*m12*m31 + m03*m11*m32 - m01*m13*m32 - m02*m11*m33 + m01*m12*m33;
    A.m03 = m03*m12*m21 - m02*m13*m21 - m03*m11*m22 + m01*m13*m22 + m02*m11*m23 - m01*m12*m23;
    A.m10 = m13*m22*m30 - m12*m23*m30 - m13*m20*m32 + m10*m23*m32 + m12*m20*m33 - m10*m22*m33;
    A.m11 = m02*m23*m30 - m03*m22*m30 + m03*m20*m32 - m00*m23*m32 - m02*m20*m33 + m00*m22*m33;
    A.m12 = m03*m12*m30 - m02*m13*m30 - m03*m10*m32 + m00*m13*m32 + m02*m10*m33 - m00*m12*m33;
    A.m13 = m02*m13*m20 - m03*m12*m20 + m03*m10*m22 - m00*m13*m22 - m02*m10*m23 + m00*m12*m23;
    A.m20 = m11*m23*m30 - m13*m21*m30 + m13*m20*m31 - m10*m23*m31 - m11*m20*m33 + m10*m21*m33;
    A.m21 = m03*m21*m30 - m01*m23*m30 - m03*m20*m31 + m00*m23*m31 + m01*m20*m33 - m00*m21*m33;
    A.m22 = m01*m13*m30 - m03*m11*m30 + m03*m10*m31 - m00*m13*m31 - m01*m10*m33 + m00*m11*m33;
    A.m23 = m03*m11*m20 - m01*m13*m20 - m03*m10*m21 + m00*m13*m21 + m01*m10*m23 - m00*m11*m23;
    A.m30 = m12*m21*m30 - m11*m22*m30 - m12*m20*m31 + m10*m22*m31 + m11*m20*m32 - m10*m21*m32;
    A.m31 = m01*m22*m30 - m02*m21*m30 + m02*m20*m31 - m00*m22*m31 - m01*m20*m32 + m00*m21*m32;
    A.m32 = m02*m11*m30 - m01*m12*m30 - m02*m10*m31 + m00*m12*m31 + m01*m10*m32 - m00*m11*m32;
    A.m33 = m01*m12*m20 - m02*m11*m20 + m02*m10*m21 - m00*m12*m21 - m01*m10*m22 + m00*m11*m22;

    float rdet = 1.f / Det();

    return A * rdet;
  }

  float& Matrix4x4::operator()(int i, int j)
  {
    return m[i][j];
  }

  const float& Matrix4x4::operator()(int i, int j) const
  {
    return m[i][j];
  }

  Matrix4x4 Matrix4x4::operator-() const
  {
    return Matrix4x4(-m00, -m01, -m02, -m03,
                     -m10, -m11, -m12, -m13,
                     -m20, -m21, -m22, -m23,
                     -m30, -m31, -m32, -m33);
  }

  Matrix4x4 Matrix4x4::operator+(const Matrix4x4& A) const
  {
    return Matrix4x4(m00 + A.m00, m01 + A.m01, m02 + A.m02, m03 + A.m03,
                     m10 + A.m10, m11 + A.m11, m12 + A.m12, m13 + A.m13,
                     m20 + A.m20, m21 + A.m21, m22 + A.m22, m23 + A.m23,
                     m30 + A.m30, m31 + A.m31, m32 + A.m32, m33 + A.m33);
  }

  Matrix4x4 Matrix4x4::operator-(const Matrix4x4& A) const
  {
    return Matrix4x4(m00 - A.m00, m01 - A.m01, m02 - A.m02, m03 - A.m03,
                     m10 - A.m10, m11 - A.m11, m12 - A.m12, m13 - A.m13,
                     m20 - A.m20, m21 - A.m21, m22 - A.m22, m23 - A.m23,
                     m30 - A.m30, m31 - A.m31, m32 - A.m32, m33 - A.m33);
  }

  Matrix4x4 Matrix4x4::operator*(float f) const
  {
    return Matrix4x4(f*m00, f*m01, f*m02, f*m03,
                     f*m10, f*m11, f*m12, f*m13,
                     f*m20, f*m21, f*m22, f*m23,
                     f*m30, f*m31, f*m32, f*m33);
  }

  Matrix4x4 Matrix4x4::operator*(const Matrix4x4& A) const
  {
    Matrix4x4 M;

    M.m00 = m00*A.m00 + m01*A.m10 + m02*A.m20 + m03*A.m30;
    M.m01 = m00*A.m01 + m01*A.m11 + m02*A.m21 + m03*A.m31;
    M.m02 = m00*A.m02 + m01*A.m12 + m02*A.m22 + m03*A.m32;
    M.m03 = m00*A.m03 + m01*A.m13 + m02*A.m23 + m03*A.m33;

    M.m10 = m10*A.m00 + m11*A.m10 + m12*A.m20 + m13*A.m30;
    M.m11 = m10*A.m01 + m11*A.m11 + m12*A.m21 + m13*A.m31;
    M.m12 = m10*A.m02 + m11*A.m12 + m12*A.m22 + m13*A.m32;
    M.m13 = m10*A.m03 + m11*A.m13 + m12*A.m23 + m13*A.m33;

    M.m20 = m20*A.m00 + m21*A.m10 + m22*A.m20 + m23*A.m30;
    M.m21 = m20*A.m01 + m21*A.m11 + m22*A.m21 + m23*A.m31;
    M.m22 = m20*A.m02 + m21*A.m12 + m22*A.m22 + m23*A.m32;
    M.m23 = m20*A.m03 + m21*A.m13 + m22*A.m23 + m23*A.m33;

    M.m30 = m30*A.m00 + m31*A.m10 + m32*A.m20 + m33*A.m30;
    M.m31 = m30*A.m01 + m31*A.m11 + m32*A.m21 + m33*A.m31;
    M.m32 = m30*A.m02 + m31*A.m12 + m32*A.m22 + m33*A.m32;
    M.m33 = m30*A.m03 + m31*A.m13 + m32*A.m23 + m33*A.m33;

    return M;
  }

  Matrix4x4 Matrix4x4::operator/(float f) const
  {
    float rf = 1.f / f;
    return Matrix4x4(rf*m00, rf*m01, rf*m02, rf*m03,
                     rf*m10, rf*m11, rf*m12, rf*m13,
                     rf*m20, rf*m21, rf*m22, rf*m23,
                     rf*m30, rf*m31, rf*m32, rf*m33);
  }

  Vector4D operator*(const Vector4D& v, const Matrix4x4& A)
  {
    return Vector4D(v.x*A(0, 0) + v.y*A(1, 0) + v.z*A(2, 0) + v.w*A(3, 0),
                    v.x*A(0, 1) + v.y*A(1, 1) + v.z*A(2, 1) + v.w*A(3, 1),
                    v.x*A(0, 2) + v.y*A(1, 2) + v.z*A(2, 2) + v.w*A(3, 2),
                    v.x*A(0, 3) + v.y*A(1, 3) + v.z*A(2, 3) + v.w*A(3, 3));
  }

  Vector3D operator*(const Vector3D& v, const Matrix4x4& A)
  {
    Vector4D v4d(v.x, v.y, v.z, 1.f);
    return (v4d*A).ProjectTo3D();
  }

  Matrix4x4 operator*(float f, const Matrix4x4& A)
  {
    return A*f;
  }

  std::ostream& operator<<(std::ostream& os, const Matrix4x4& A)
  {
    for (int i = 0; i < 4; i++)
    {
      os << "[";
      for (int j = 0; j < 4; j++)
      {
        os << A(i, j) << ",";
      }
      os << "]" << std::endl;
    }
    return os;
  }
}