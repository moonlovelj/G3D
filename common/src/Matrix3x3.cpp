#include "Matrix3x3.h"

namespace g3dcommon {

  Matrix3x3 Matrix3x3::Identity()
  {
    Matrix3x3 A;
    A(0, 0) = 1.f; A(0, 1) = 0.f; A(0, 2) = 0.f;
    A(1, 0) = 0.f; A(1, 1) = 1.f; A(1, 2) = 0.f;
    A(2, 0) = 0.f; A(2, 1) = 0.f; A(2, 2) = 1.f;
    return A;
  }

  float Matrix3x3::Det() const
  {
    return -m02*m11*m20 + m01*m12*m20 +
            m02*m10*m21 - m00*m12*m21 -
            m01*m10*m22 + m00*m11*m22;
  }

  Matrix3x3 Matrix3x3::T() const
  {
    Matrix3x3 A(m00, m10, m20,
                m01, m11, m21,
                m02, m12, m22);

    return A;
  }

  Matrix3x3 Matrix3x3::Inv() const
  {
    // A is standard adjoint matrix.
    Matrix3x3 A;
    A.m00 = m11*m22 - m21*m12; A.m01 = m21*m02 - m01*m22; A.m02 = m01*m12 - m11*m02;
    A.m10 = m20*m12 - m10*m22; A.m11 = m00*m22 - m20*m02; A.m12 = m10*m02 - m00*m12;
    A.m20 = m10*m21 - m20*m11; A.m21 = m20*m01 - m00*m21; A.m22 = m00*m11 - m10*m01;

    float rdet = 1.f / Det();

    return A * rdet;
  }

  float& Matrix3x3::operator()(int i, int j)
  {
    return m[i][j];
  }

  const float& Matrix3x3::operator()(int i, int j) const
  {
    return m[i][j];
  }

  Matrix3x3 Matrix3x3::operator-() const
  {
    return Matrix3x3(-m00, -m01, -m02,
                     -m10, -m11, -m12,
                     -m20, -m21, -m22);
  }

  Matrix3x3 Matrix3x3::operator+(const Matrix3x3& A) const
  {
    return Matrix3x3(m00 + A.m00, m01 + A.m01, m02 + A.m02,
                     m10 + A.m10, m11 + A.m11, m12 + A.m12,
                     m20 + A.m20, m21 + A.m21, m22 + A.m22);
  }

  Matrix3x3 Matrix3x3::operator-(const Matrix3x3& A) const
  {
    return Matrix3x3(m00 - A.m00, m01 - A.m01, m02 - A.m02,
                     m10 - A.m10, m11 - A.m11, m12 - A.m12,
                     m20 - A.m20, m21 - A.m21, m22 - A.m22);
  }

  Matrix3x3 Matrix3x3::operator*(float f) const
  {
    return Matrix3x3(f*m00, f*m01, f*m02,
                     f*m10, f*m11, f*m12,
                     f*m20, f*m21, f*m22);
  }

  Matrix3x3 Matrix3x3::operator*(const Matrix3x3& A) const
  {
    Matrix3x3 M;
    M.m00 = m00*A.m00 + m01*A.m10 + m02*A.m20; M.m01 = m00*A.m01 + m01*A.m11 + m02*A.m21; M.m02 = m00*A.m02 + m01*A.m12 + m02*A.m22;
    M.m10 = m10*A.m00 + m11*A.m10 + m12*A.m20; M.m11 = m10*A.m01 + m11*A.m11 + m12*A.m21; M.m12 = m10*A.m02 + m11*A.m12 + m12*A.m22;
    M.m20 = m20*A.m00 + m21*A.m10 + m22*A.m20; M.m21 = m20*A.m01 + m21*A.m11 + m22*A.m21; M.m22 = m20*A.m02 + m21*A.m12 + m22*A.m22;

    return M;
  }

  Matrix3x3 Matrix3x3::operator/(float f) const
  {
    float rf = 1.f / f;
    return Matrix3x3(rf*m00, rf*m01, rf*m02,
                     rf*m10, rf*m11, rf*m12,
                     rf*m20, rf*m21, rf*m22);
  }

  Vector3D operator*(const Vector3D& v, const Matrix3x3& A)
  {
    return Vector3D(v.x*A(0, 0) + v.y*A(1, 0) + v.z*A(2, 0),
                    v.x*A(0, 1) + v.y*A(1, 1) + v.z*A(2, 1), 
                    v.x*A(0, 2) + v.y*A(1, 2) + v.z*A(2, 2));
  }

  Matrix3x3 operator*(float f, const Matrix3x3& A)
  {
    return A*f;
  }

  std::ostream& operator<<(std::ostream& os, const Matrix3x3& A)
  {
    for (int i = 0; i < 3; i++)
    {
      os << "[";
      for (int j = 0; j < 3; j++)
      {
        os << A(i, j) << ",";
      }
      os << "]" << std::endl;
    }
    return os;
  }
}